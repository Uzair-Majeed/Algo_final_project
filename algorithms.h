/**
 * Core Algorithms for Disaster Relief Optimization - C++ Implementation
 * Implements Dijkstra, A*, Dynamic Programming, and Greedy algorithms
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h"
#include <vector>
#include <unordered_map>
#include <limits>
#include <queue>
#include <functional>

using namespace std;

namespace DisasterRelief {

/**
 * Shortest Path Algorithms
 */
class ShortestPathAlgorithms {
public:
    /**
     * Dijkstra's algorithm for shortest paths from source to all nodes
     * Time Complexity: O((V + E) log V)
     * Space Complexity: O(V)
     */
    static pair<unordered_map<int, double>, unordered_map<int, int>>
    dijkstra(const Graph& graph, int source, bool useReliability = true);
    
    /**
     * Reconstruct path from parent pointers
     */
    static vector<int> reconstructPath(
        const unordered_map<int, int>& parents,
        int start, int goal
    );
    
    /**
     * Calculate Euclidean distance heuristic
     */
    static double euclideanDistance(const Graph& graph, int node1, int node2);
    
    /**
     * A* search algorithm for optimal path with heuristic
     * Time Complexity: O(E log V) with good heuristic
     * Space Complexity: O(V)
     */
    static pair<vector<int>, double>
    aStar(const Graph& graph, int start, int goal);
};

/**
 * Dynamic Programming Algorithms
 */
class DynamicProgramming {
public:
    /**
     * 0/1 Knapsack for selecting optimal locations given capacity constraint
     * Time Complexity: O(n × C) where n = number of locations, C = capacity
     * Space Complexity: O(n × C)
     */
    static pair<vector<int>, int>
    knapsackCapacityOptimization(
        const vector<int>& locations,
        const Graph& graph,
        int capacity
    );
};

/**
 * Greedy Algorithms
 */
class GreedyAlgorithms {
public:
    /**
     * Greedy route construction based on priority and distance
     * Time Complexity: O(K × V²) where K = vehicles, V = nodes
     * Space Complexity: O(K × V)
     */
    static unordered_map<int, vector<int>>
    priorityGreedyRouting(
        const Graph& graph,
        vector<Vehicle>& vehicles,
        int depot
    );
    
    /**
     * Nearest neighbor heuristic for TSP-like route ordering
     * Time Complexity: O(V²)
     * Space Complexity: O(V)
     */
    static vector<int>
    nearestNeighborTSP(
        const Graph& graph,
        const vector<int>& locations,
        int start
    );
};

/**
 * Multi-Objective Optimization
 */
class MultiObjectiveOptimization {
public:
    /**
     * Calculate multi-objective score for a route
     * Minimize: α × Σ(priority × time) + β × Σ(1 - reliability) + γ × idle_penalty
     */
    static double calculateRouteScore(
        const Graph& graph,
        const vector<int>& route,
        double alpha = 1.0,
        double beta = 1.0,
        double gamma = 1.0
    );
    
    /**
     * Solution evaluation metrics
     */
    struct SolutionMetrics {
        double totalScore;
        double totalDistance;
        int locationsServed;
        int totalPriority;
        unordered_map<int, vector<int>> routes;
    };
    
    /**
     * Comprehensive evaluation of a solution
     */
    static SolutionMetrics evaluateSolution(
        const Graph& graph,
        const unordered_map<int, vector<int>>& routes,
        const vector<Vehicle>& vehicles,
        double alpha = 1.0,
        double beta = 1.0,
        double gamma = 1.0
    );
};

} // namespace DisasterRelief

#endif // ALGORITHMS_H

