/**
 * Graph Module for Disaster Relief Optimization - C++ Implementation
 * Handles graph representation, node/edge management, and basic graph operations
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include <cmath>

using namespace std;

namespace DisasterRelief {

/**
 * Node represents a location in the disaster area
 */
struct Node {
    int id;
    int demand;
    int priority;
    double x;
    double y;
    
    Node() : id(0), demand(0), priority(0), x(0.0), y(0.0) {}
    Node(int id, int demand, int priority, double x = 0.0, double y = 0.0)
        : id(id), demand(demand), priority(priority), x(x), y(y) {}
};

/**
 * Edge represents a road connection between locations
 */
struct Edge {
    int u;
    int v;
    double cost;
    double reliability;
    
    Edge() : u(0), v(0), cost(0.0), reliability(1.0) {}
    Edge(int u, int v, double cost, double reliability)
        : u(u), v(v), cost(cost), reliability(reliability) {}
    
    // Calculate cost penalized by unreliability
    double getWeightedCost() const {
        return cost * (2.0 - reliability);
    }
};

/**
 * Neighbor structure for adjacency list
 */
struct Neighbor {
    int nodeId;
    double cost;
    double reliability;
    
    Neighbor(int id, double c, double r) 
        : nodeId(id), cost(c), reliability(r) {}
};

/**
 * Vehicle represents a rescue vehicle
 */
struct Vehicle {
    int id;
    int capacity;
    double maxTime;
    int currentLoad;
    vector<int> route;
    
    Vehicle() : id(0), capacity(0), maxTime(INFINITY), currentLoad(0) {}
    Vehicle(int id, int capacity) 
        : id(id), capacity(capacity), maxTime(INFINITY), currentLoad(0) {}
    
    int remainingCapacity() const {
        return capacity - currentLoad;
    }
    
    bool canServe(int demand) const {
        return currentLoad + demand <= capacity;
    }
};

/**
 * Graph class - Weighted undirected graph for disaster relief optimization
 * Uses adjacency list representation for efficiency
 */
class Graph {
private:
    unordered_map<int, Node> nodes;
    unordered_map<int, vector<Neighbor>> adjacencyList;
    vector<Edge> edges;
    
public:
    Graph() = default;
    
    // Add node to the graph
    void addNode(const Node& node);
    
    // Add undirected edge to the graph
    void addEdge(const Edge& edge);
    
    // Get all neighbors of a node
    const vector<Neighbor>& getNeighbors(int nodeId) const;
    
    // Get node by ID
    optional<Node> getNode(int nodeId) const;
    
    // Get number of nodes
    size_t numNodes() const { return nodes.size(); }
    
    // Get number of edges
    size_t numEdges() const { return edges.size(); }
    
    // Get all node IDs
    vector<int> getAllNodeIds() const;
    
    // Get edge cost between two nodes
    optional<double> getEdgeCost(int u, int v, bool useReliability = true) const;
    
    // Remove edge (for dynamic replanning)
    void removeEdge(int u, int v);
    
    // Load from JSON file
    static Graph fromJsonFile(const string& filename);
    
    // Save to JSON file
    void saveToJsonFile(const string& filename) const;
    
    // Get all nodes
    const unordered_map<int, Node>& getNodes() const { return nodes; }
    
    // Get all edges
    const vector<Edge>& getEdges() const { return edges; }
};

} // namespace DisasterRelief

#endif // GRAPH_H

