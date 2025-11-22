/**
 * Core Algorithms Implementation
 */

#include "algorithms.h"
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <iostream>

using namespace std;

namespace DisasterRelief {

// ============================================================================
// Shortest Path Algorithms
// ============================================================================

pair<unordered_map<int, double>, unordered_map<int, int>>
ShortestPathAlgorithms::dijkstra(const Graph& graph, int source, bool useReliability) {
    unordered_map<int, double> distances;
    unordered_map<int, int> parents;
    unordered_set<int> visited;
    
    // Initialize distances
    for (int nodeId : graph.getAllNodeIds()) {
        distances[nodeId] = numeric_limits<double>::infinity();
        parents[nodeId] = -1;
    }
    distances[source] = 0.0;
    
    // Priority queue: (distance, node_id)
    using PQElement = pair<double, int>;
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> pq;
    pq.push({0.0, source});
    
    while (!pq.empty()) {
        auto [currentDist, u] = pq.top();
        pq.pop();
        
        if (visited.count(u)) continue;
        visited.insert(u);
        
        // Relaxation step
        for (const auto& neighbor : graph.getNeighbors(u)) {
            int v = neighbor.nodeId;
            if (visited.count(v)) continue;
            
            // Calculate edge weight
            double edgeWeight;
            if (useReliability) {
                edgeWeight = neighbor.cost * (2.0 - neighbor.reliability);
            } else {
                edgeWeight = neighbor.cost;
            }
            
            double alt = distances[u] + edgeWeight;
            
            if (alt < distances[v]) {
                distances[v] = alt;
                parents[v] = u;
                pq.push({alt, v});
            }
        }
    }
    
    return {distances, parents};
}

vector<int> ShortestPathAlgorithms::reconstructPath(
    const unordered_map<int, int>& parents,
    int start, int goal
) {
    if (parents.at(goal) == -1 && start != goal) {
        return {}; // No path exists
    }
    
    vector<int> path;
    int current = goal;
    
    while (current != -1) {
        path.push_back(current);
        auto it = parents.find(current);
        if (it == parents.end()) break;
        current = it->second;
    }
    
    reverse(path.begin(), path.end());
    return path;
}

double ShortestPathAlgorithms::euclideanDistance(
    const Graph& graph, int node1, int node2
) {
    auto n1 = graph.getNode(node1);
    auto n2 = graph.getNode(node2);
    
    if (!n1 || !n2) return 0.0;
    
    double dx = n1->x - n2->x;
    double dy = n1->y - n2->y;
    return sqrt(dx * dx + dy * dy);
}

pair<vector<int>, double>
ShortestPathAlgorithms::aStar(const Graph& graph, int start, int goal) {
    unordered_map<int, double> gScore;
    unordered_map<int, double> fScore;
    unordered_map<int, int> cameFrom;
    
    // Initialize scores
    for (int nodeId : graph.getAllNodeIds()) {
        gScore[nodeId] = numeric_limits<double>::infinity();
        fScore[nodeId] = numeric_limits<double>::infinity();
    }
    gScore[start] = 0.0;
    fScore[start] = euclideanDistance(graph, start, goal);
    
    // Open set with priority queue
    using PQElement = pair<double, int>;
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> openSet;
    unordered_set<int> openSetHash;
    
    openSet.push({fScore[start], start});
    openSetHash.insert(start);
    
    while (!openSet.empty()) {
        auto [_, current] = openSet.top();
        openSet.pop();
        
        if (!openSetHash.count(current)) continue;
        openSetHash.erase(current);
        
        if (current == goal) {
            // Reconstruct path
            vector<int> path = {current};
            while (cameFrom.count(current)) {
                current = cameFrom[current];
                path.push_back(current);
            }
            reverse(path.begin(), path.end());
            return {path, gScore[goal]};
        }
        
        for (const auto& neighbor : graph.getNeighbors(current)) {
            int neighborId = neighbor.nodeId;
            double edgeWeight = neighbor.cost * (2.0 - neighbor.reliability);
            double tentativeG = gScore[current] + edgeWeight;
            
            if (tentativeG < gScore[neighborId]) {
                cameFrom[neighborId] = current;
                gScore[neighborId] = tentativeG;
                fScore[neighborId] = tentativeG + euclideanDistance(graph, neighborId, goal);
                
                if (!openSetHash.count(neighborId)) {
                    openSet.push({fScore[neighborId], neighborId});
                    openSetHash.insert(neighborId);
                }
            }
        }
    }
    
    return {{}, numeric_limits<double>::infinity()};
}

// ============================================================================
// Dynamic Programming
// ============================================================================

pair<vector<int>, int>
DynamicProgramming::knapsackCapacityOptimization(
    const vector<int>& locations,
    const Graph& graph,
    int capacity
) {
    int n = locations.size();
    if (n == 0 || capacity == 0) {
        return {{}, 0};
    }
    
    // DP table: dp[i][w] = max priority using first i items with capacity w
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    
    // Fill DP table
    for (int i = 1; i <= n; i++) {
        int locId = locations[i - 1];
        auto nodeOpt = graph.getNode(locId);
        if (!nodeOpt) continue;
        
        int demand = nodeOpt->demand;
        int priority = nodeOpt->priority;
        
        for (int w = 0; w <= capacity; w++) {
            // Don't take item
            dp[i][w] = dp[i-1][w];
            
            // Take item if possible
            if (demand <= w) {
                dp[i][w] = max(dp[i][w], dp[i-1][w - demand] + priority);
            }
        }
    }
    
    // Backtrack to find selected items
    vector<int> selected;
    int w = capacity;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            selected.push_back(locations[i-1]);
            auto nodeOpt = graph.getNode(locations[i-1]);
            if (nodeOpt) {
                w -= nodeOpt->demand;
            }
        }
    }
    
    reverse(selected.begin(), selected.end());
    return {selected, dp[n][capacity]};
}

// ============================================================================
// Greedy Algorithms
// ============================================================================

unordered_map<int, vector<int>>
GreedyAlgorithms::priorityGreedyRouting(
    const Graph& graph,
    vector<Vehicle>& vehicles,
    int depot
) {
    // Compute shortest paths from depot
    auto [distances, parents] = ShortestPathAlgorithms::dijkstra(graph, depot, true);
    
    // Get all non-depot nodes sorted by priority (descending)
    vector<int> locations;
    for (int nodeId : graph.getAllNodeIds()) {
        if (nodeId != depot) {
            locations.push_back(nodeId);
        }
    }
    
    sort(locations.begin(), locations.end(), [&graph](int a, int b) {
        auto nodeA = graph.getNode(a);
        auto nodeB = graph.getNode(b);
        if (!nodeA || !nodeB) return false;
        return nodeA->priority > nodeB->priority;
    });
    
    unordered_set<int> visited;
    unordered_map<int, vector<int>> routes;
    
    // Initialize routes with depot
    for (auto& vehicle : vehicles) {
        routes[vehicle.id] = {depot};
        vehicle.currentLoad = 0;
    }
    
    // Assign locations to vehicles greedily
    for (auto& vehicle : vehicles) {
        while (true) {
            int bestNode = -1;
            double bestScore = -numeric_limits<double>::infinity();
            
            // Find best unvisited location
            for (int locId : locations) {
                if (visited.count(locId)) continue;
                
                auto nodeOpt = graph.getNode(locId);
                if (!nodeOpt) continue;
                
                // Check capacity constraint
                if (vehicle.currentLoad + nodeOpt->demand > vehicle.capacity) {
                    continue;
                }
                
                // Calculate score: priority / (distance + 1)
                double dist = distances[locId];
                if (isinf(dist)) continue;
                
                double score = nodeOpt->priority / (dist + 1.0);
                
                if (score > bestScore) {
                    bestScore = score;
                    bestNode = locId;
                }
            }
            
            if (bestNode == -1) break;
            
            // Add to route
            routes[vehicle.id].push_back(bestNode);
            visited.insert(bestNode);
            
            auto nodeOpt = graph.getNode(bestNode);
            if (nodeOpt) {
                vehicle.currentLoad += nodeOpt->demand;
            }
        }
        
        // Return to depot
        routes[vehicle.id].push_back(depot);
    }
    
    return routes;
}

vector<int>
GreedyAlgorithms::nearestNeighborTSP(
    const Graph& graph,
    const vector<int>& locations,
    int start
) {
    if (locations.empty()) {
        return {start};
    }
    
    vector<int> route = {start};
    unordered_set<int> unvisited(locations.begin(), locations.end());
    int current = start;
    
    while (!unvisited.empty()) {
        int nearest = -1;
        double minDist = numeric_limits<double>::infinity();
        
        for (int loc : unvisited) {
            auto costOpt = graph.getEdgeCost(current, loc);
            if (costOpt && *costOpt < minDist) {
                minDist = *costOpt;
                nearest = loc;
            }
        }
        
        if (nearest == -1) {
            // No path found, just add remaining
            for (int loc : unvisited) {
                route.push_back(loc);
            }
            break;
        }
        
        route.push_back(nearest);
        unvisited.erase(nearest);
        current = nearest;
    }
    
    return route;
}

// ============================================================================
// Multi-Objective Optimization
// ============================================================================

double MultiObjectiveOptimization::calculateRouteScore(
    const Graph& graph,
    const vector<int>& route,
    double alpha,
    double beta,
    double gamma
) {
    if (route.size() <= 1) return 0.0;
    
    double priorityTime = 0.0;
    double unreliability = 0.0;
    double totalTime = 0.0;
    
    // Calculate priority-weighted time and unreliability
    for (size_t i = 0; i < route.size() - 1; i++) {
        int u = route[i];
        int v = route[i + 1];
        
        // Get edge info
        double edgeCost = 0.0;
        double edgeReliability = 1.0;
        
        for (const auto& neighbor : graph.getNeighbors(u)) {
            if (neighbor.nodeId == v) {
                edgeCost = neighbor.cost;
                edgeReliability = neighbor.reliability;
                break;
            }
        }
        
        totalTime += edgeCost;
        unreliability += (1.0 - edgeReliability);
        
        // Priority-time for destination
        auto nodeOpt = graph.getNode(v);
        if (nodeOpt && nodeOpt->priority > 0) {
            priorityTime += nodeOpt->priority * totalTime;
        }
    }
    
    double idlePenalty = 0.0;
    double totalScore = alpha * priorityTime + beta * unreliability + gamma * idlePenalty;
    
    return totalScore;
}

MultiObjectiveOptimization::SolutionMetrics
MultiObjectiveOptimization::evaluateSolution(
    const Graph& graph,
    const unordered_map<int, vector<int>>& routes,
    const vector<Vehicle>& vehicles,
    double alpha,
    double beta,
    double gamma
) {
    SolutionMetrics metrics;
    metrics.totalScore = 0.0;
    metrics.totalDistance = 0.0;
    metrics.locationsServed = 0;
    metrics.totalPriority = 0;
    metrics.routes = routes;
    
    unordered_set<int> locationsServed;
    
    for (const auto& vehicle : vehicles) {
        auto it = routes.find(vehicle.id);
        if (it == routes.end()) continue;
        
        const auto& route = it->second;
        
        // Calculate route score
        double routeScore = calculateRouteScore(graph, route, alpha, beta, gamma);
        metrics.totalScore += routeScore;
        
        // Calculate distance
        for (size_t i = 0; i < route.size() - 1; i++) {
            auto costOpt = graph.getEdgeCost(route[i], route[i + 1], false);
            if (costOpt) {
                metrics.totalDistance += *costOpt;
            }
        }
        
        // Count served locations
        for (int nodeId : route) {
            auto nodeOpt = graph.getNode(nodeId);
            if (nodeOpt && nodeOpt->priority > 0 && !locationsServed.count(nodeId)) {
                locationsServed.insert(nodeId);
                metrics.totalPriority += nodeOpt->priority;
            }
        }
    }
    
    metrics.locationsServed = locationsServed.size();
    
    return metrics;
}

} // namespace DisasterRelief

