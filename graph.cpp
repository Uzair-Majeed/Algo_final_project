/**
 * Graph Module Implementation
 */

#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace DisasterRelief {

// Static empty neighbor list for missing nodes
static const vector<Neighbor> EMPTY_NEIGHBORS;

void Graph::addNode(const Node& node) {
    nodes[node.id] = node;
    if (adjacencyList.find(node.id) == adjacencyList.end()) {
        adjacencyList[node.id] = vector<Neighbor>();
    }
}

void Graph::addEdge(const Edge& edge) {
    edges.push_back(edge);
    
    // Ensure nodes exist in adjacency list
    if (adjacencyList.find(edge.u) == adjacencyList.end()) {
        adjacencyList[edge.u] = vector<Neighbor>();
    }
    if (adjacencyList.find(edge.v) == adjacencyList.end()) {
        adjacencyList[edge.v] = vector<Neighbor>();
    }
    
    // Add both directions (undirected graph)
    adjacencyList[edge.u].emplace_back(edge.v, edge.cost, edge.reliability);
    adjacencyList[edge.v].emplace_back(edge.u, edge.cost, edge.reliability);
}

const vector<Neighbor>& Graph::getNeighbors(int nodeId) const {
    auto it = adjacencyList.find(nodeId);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    return EMPTY_NEIGHBORS;
}

const Node* Graph::getNode(int nodeId) const {
    auto it = nodes.find(nodeId);
    if (it != nodes.end()) {
        return &(it->second);
    }
    return nullptr;
}

vector<int> Graph::getAllNodeIds() const {
    vector<int> ids;
    ids.reserve(nodes.size());
    for (const auto& pair : nodes) {
        ids.push_back(pair.first);
    }
    return ids;
}

EdgeCostResult Graph::getEdgeCost(int u, int v, bool useReliability) const {
    auto it = adjacencyList.find(u);
    if (it == adjacencyList.end()) {
        return EdgeCostResult();
    }
    
    for (const auto& neighbor : it->second) {
        if (neighbor.nodeId == v) {
            if (useReliability) {
                return EdgeCostResult(neighbor.cost * (2.0 - neighbor.reliability));
            }
            return EdgeCostResult(neighbor.cost);
        }
    }
    
    return EdgeCostResult();
}

void Graph::removeEdge(int u, int v) {
    // Remove from adjacency list
    auto it1 = adjacencyList.find(u);
    if (it1 != adjacencyList.end()) {
        auto& neighbors = it1->second;
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                [v](const Neighbor& n) { return n.nodeId == v; }),
            neighbors.end()
        );
    }
    
    auto it2 = adjacencyList.find(v);
    if (it2 != adjacencyList.end()) {
        auto& neighbors = it2->second;
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                [u](const Neighbor& n) { return n.nodeId == u; }),
            neighbors.end()
        );
    }
    
    // Remove from edge list
    edges.erase(
        remove_if(edges.begin(), edges.end(),
            [u, v](const Edge& e) {
                return (e.u == u && e.v == v) || (e.u == v && e.v == u);
            }),
        edges.end()
    );
}

// Simple JSON parser for our specific format
Graph Graph::fromJsonFile(const string& filename) {
    Graph graph;
    ifstream file(filename);
    
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    
    string line;
    string content;
    while (getline(file, line)) {
        content += line;
    }
    file.close();
    
    // Simple JSON parsing (assumes well-formed input)
    size_t pos = 0;
    
    // Parse nodes
    pos = content.find("\"nodes\"");
    if (pos != string::npos) {
        size_t start = content.find('[', pos);
        size_t end = content.find(']', start);
        string nodesStr = content.substr(start + 1, end - start - 1);
        
        // Parse each node
        size_t nodeStart = 0;
        while ((nodeStart = nodesStr.find('{', nodeStart)) != string::npos) {
            size_t nodeEnd = nodesStr.find('}', nodeStart);
            string nodeStr = nodesStr.substr(nodeStart, nodeEnd - nodeStart + 1);
            
            Node node;
            // Extract id
            size_t idPos = nodeStr.find("\"id\"");
            if (idPos != string::npos) {
                size_t colonPos = nodeStr.find(':', idPos);
                size_t commaPos = nodeStr.find_first_of(",}", colonPos);
                node.id = stoi(nodeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract demand
            size_t demandPos = nodeStr.find("\"demand\"");
            if (demandPos != string::npos) {
                size_t colonPos = nodeStr.find(':', demandPos);
                size_t commaPos = nodeStr.find_first_of(",}", colonPos);
                node.demand = stoi(nodeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract priority
            size_t priorityPos = nodeStr.find("\"priority\"");
            if (priorityPos != string::npos) {
                size_t colonPos = nodeStr.find(':', priorityPos);
                size_t commaPos = nodeStr.find_first_of(",}", colonPos);
                node.priority = stoi(nodeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract x (optional)
            size_t xPos = nodeStr.find("\"x\"");
            if (xPos != string::npos) {
                size_t colonPos = nodeStr.find(':', xPos);
                size_t commaPos = nodeStr.find_first_of(",}", colonPos);
                node.x = stod(nodeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract y (optional)
            size_t yPos = nodeStr.find("\"y\"");
            if (yPos != string::npos) {
                size_t colonPos = nodeStr.find(':', yPos);
                size_t commaPos = nodeStr.find_first_of(",}", colonPos);
                node.y = stod(nodeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            graph.addNode(node);
            nodeStart = nodeEnd + 1;
        }
    }
    
    // Parse edges
    pos = content.find("\"edges\"");
    if (pos != string::npos) {
        size_t start = content.find('[', pos);
        size_t end = content.find(']', start);
        string edgesStr = content.substr(start + 1, end - start - 1);
        
        // Parse each edge
        size_t edgeStart = 0;
        while ((edgeStart = edgesStr.find('{', edgeStart)) != string::npos) {
            size_t edgeEnd = edgesStr.find('}', edgeStart);
            string edgeStr = edgesStr.substr(edgeStart, edgeEnd - edgeStart + 1);
            
            Edge edge;
            // Extract u
            size_t uPos = edgeStr.find("\"u\"");
            if (uPos != string::npos) {
                size_t colonPos = edgeStr.find(':', uPos);
                size_t commaPos = edgeStr.find(',', colonPos);
                edge.u = stoi(edgeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract v
            size_t vPos = edgeStr.find("\"v\"");
            if (vPos != string::npos) {
                size_t colonPos = edgeStr.find(':', vPos);
                size_t commaPos = edgeStr.find(',', colonPos);
                edge.v = stoi(edgeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract cost
            size_t costPos = edgeStr.find("\"cost\"");
            if (costPos != string::npos) {
                size_t colonPos = edgeStr.find(':', costPos);
                size_t commaPos = edgeStr.find(',', colonPos);
                edge.cost = stod(edgeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            // Extract reliability
            size_t reliabilityPos = edgeStr.find("\"reliability\"");
            if (reliabilityPos != string::npos) {
                size_t colonPos = edgeStr.find(':', reliabilityPos);
                size_t commaPos = edgeStr.find_first_of(",}", colonPos);
                edge.reliability = stod(edgeStr.substr(colonPos + 1, commaPos - colonPos - 1));
            }
            
            graph.addEdge(edge);
            edgeStart = edgeEnd + 1;
        }
    }
    
    return graph;
}

void Graph::saveToJsonFile(const string& filename) const {
    ofstream file(filename);
    
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + filename);
    }
    
    file << "{\n  \"nodes\": [\n";
    
    bool firstNode = true;
    for (const auto& pair : nodes) {
        const Node& node = pair.second;
        if (!firstNode) file << ",\n";
        firstNode = false;
        
        file << "    {\"id\": " << node.id 
             << ", \"demand\": " << node.demand
             << ", \"priority\": " << node.priority
             << ", \"x\": " << node.x
             << ", \"y\": " << node.y << "}";
    }
    
    file << "\n  ],\n  \"edges\": [\n";
    
    bool firstEdge = true;
    for (const auto& edge : edges) {
        if (!firstEdge) file << ",\n";
        firstEdge = false;
        
        file << "    {\"u\": " << edge.u
             << ", \"v\": " << edge.v
             << ", \"cost\": " << edge.cost
             << ", \"reliability\": " << edge.reliability << "}";
    }
    
    file << "\n  ]\n}\n";
    file.close();
}

} // namespace DisasterRelief

