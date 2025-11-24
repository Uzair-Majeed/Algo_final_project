#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

struct Node {
    int id;
    int demand;
    int priority;
    Node() : id(0), demand(0), priority(0) {}
    Node(int id, int demand, int priority) : id(id), demand(demand), priority(priority) {}
};

struct Edge {
    int u;
    int v;
    double cost;
    double reliability;
    Edge() : u(0), v(0), cost(0.0), reliability(1.0) {}
    Edge(int u, int v, double cost, double reliability) : u(u), v(v), cost(cost), reliability(reliability) {}
};

class Graph {
private:
    unordered_map<int, Node> nodes;
    unordered_map<int, vector<pair<int, Edge>>> adjacencyList;
    vector<Edge> edges;

public:
    Graph() = default;

    void addNode(const Node& node) {
        nodes[node.id] = node;
        if (adjacencyList.find(node.id) == adjacencyList.end()) {
            adjacencyList[node.id] = vector<pair<int, Edge>>();
        }
    }

    void addEdge(const Edge& edge) {
        edges.push_back(edge);
        adjacencyList[edge.u].push_back({edge.v, edge});
        adjacencyList[edge.v].push_back({edge.u, edge});
    }

    vector<pair<int, Edge>> getNeighbors(int nodeId) const {
        auto it = adjacencyList.find(nodeId);
        
        if (it != adjacencyList.end()) return it->second;
        
        return {};
    }

    const Node* getNode(int nodeId) const {
        auto it = nodes.find(nodeId);
        if (it != nodes.end()) return &(it->second);
        return nullptr;
    }

    Node* getNode(int nodeId) {
        auto it = nodes.find(nodeId);
        if (it != nodes.end()) return &(it->second);
        return nullptr;
    }

    vector<int> getAllNodeIds() const {
        vector<int> ids;
        for (const auto& pair : nodes) ids.push_back(pair.first);
        return ids;
    }

    double getEdgeCost(int u, int v) const {
        for (const auto& neighbor : getNeighbors(u)) {
            if (neighbor.first == v) return neighbor.second.cost;
        }
        return -1.0;
    }

    double getEdgeReliability(int u, int v) const {
        for (const auto& neighbor : getNeighbors(u)) {
            if (neighbor.first == v) return neighbor.second.reliability;
        }
        return 0.0;
    }

    const vector<Edge>& getEdges() const { return edges; }
    int numNodes() const { return nodes.size(); }
    int numEdges() const { return edges.size(); }
};

#endif