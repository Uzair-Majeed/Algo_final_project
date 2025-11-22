/**
 * Solver Implementation
 */

#include "solver.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

namespace DisasterRelief {

DisasterReliefSolver::DisasterReliefSolver(
    const Graph& g,
    const vector<Vehicle>& v,
    int depot,
    double alpha,
    double beta,
    double gamma
) : graph(g), vehicles(v), depot(depot), alpha(alpha), beta(beta), gamma(gamma),
    computationTime(0.0), dijkstraTime(0.0), routeConstructionTime(0.0), optimizationTime(0.0) {
}

MultiObjectiveOptimization::SolutionMetrics
DisasterReliefSolver::solve(bool useOptimization) {
    auto startTime = chrono::high_resolution_clock::now();
    
    // Phase 1: Preprocessing - Compute shortest paths from depot
    cout << "Phase 1: Computing shortest paths from depot..." << endl;
    auto dijkstraStart = chrono::high_resolution_clock::now();
    
    auto [distances, parents] = ShortestPathAlgorithms::dijkstra(graph, depot, true);
    distancesFromDepot = distances;
    parentsFromDepot = parents;
    
    auto dijkstraEnd = chrono::high_resolution_clock::now();
    dijkstraTime = chrono::duration<double>(dijkstraEnd - dijkstraStart).count();
    cout << "  Completed in " << fixed << setprecision(4) 
              << dijkstraTime << " seconds" << endl;
    
    // Phase 2: Initial Route Construction using Greedy Algorithm
    cout << "\nPhase 2: Constructing initial routes (Greedy)..." << endl;
    auto routeStart = chrono::high_resolution_clock::now();
    
    routes = GreedyAlgorithms::priorityGreedyRouting(graph, vehicles, depot);
    
    auto routeEnd = chrono::high_resolution_clock::now();
    routeConstructionTime = chrono::duration<double>(routeEnd - routeStart).count();
    cout << "  Completed in " << fixed << setprecision(4) 
              << routeConstructionTime << " seconds" << endl;
    
    // Phase 3: Route Optimization using Dynamic Programming
    if (useOptimization) {
        cout << "\nPhase 3: Optimizing routes (Dynamic Programming)..." << endl;
        auto optStart = chrono::high_resolution_clock::now();
        
        optimizeRoutesDP();
        
        auto optEnd = chrono::high_resolution_clock::now();
        optimizationTime = chrono::duration<double>(optEnd - optStart).count();
        cout << "  Completed in " << fixed << setprecision(4) 
                  << optimizationTime << " seconds" << endl;
    }
    
    // Phase 4: Evaluation
    cout << "\nPhase 4: Evaluating solution..." << endl;
    auto solution = MultiObjectiveOptimization::evaluateSolution(
        graph, routes, vehicles, alpha, beta, gamma
    );
    
    auto endTime = chrono::high_resolution_clock::now();
    computationTime = chrono::duration<double>(endTime - startTime).count();
    
    return solution;
}

void DisasterReliefSolver::optimizeRoutesDP() {
    for (auto& vehicle : vehicles) {
        auto it = routes.find(vehicle.id);
        if (it == routes.end()) continue;
        
        auto& route = it->second;
        
        // Extract locations (exclude depot)
        vector<int> locations;
        for (int nodeId : route) {
            if (nodeId != depot) {
                locations.push_back(nodeId);
            }
        }
        
        if (locations.empty()) continue;
        
        // Apply knapsack DP to select optimal subset
        auto [selectedLocations, totalPriority] = 
            DynamicProgramming::knapsackCapacityOptimization(locations, graph, vehicle.capacity);
        
        // Reorder using nearest neighbor for better route
        if (!selectedLocations.empty()) {
            vector<int> optimizedRoute = 
                GreedyAlgorithms::nearestNeighborTSP(graph, selectedLocations, depot);
            optimizedRoute.push_back(depot); // Return to depot
            routes[vehicle.id] = optimizedRoute;
        }
    }
}

vector<int> DisasterReliefSolver::replanRoute(
    int vehicleId,
    int currentLocation,
    const vector<pair<int, int>>& blockedEdges
) {
    cout << "\nReplanning route for vehicle " << vehicleId << "..." << endl;
    
    // Temporarily remove blocked edges
    for (const auto& [u, v] : blockedEdges) {
        graph.removeEdge(u, v);
    }
    
    // Get remaining locations in route
    auto it = routes.find(vehicleId);
    if (it == routes.end()) {
        return {currentLocation, depot};
    }
    
    const auto& currentRoute = it->second;
    auto currentIt = find(currentRoute.begin(), currentRoute.end(), currentLocation);
    
    vector<int> remainingLocations;
    if (currentIt != currentRoute.end()) {
        for (auto iter = currentIt + 1; iter != currentRoute.end(); ++iter) {
            if (*iter != depot) {
                remainingLocations.push_back(*iter);
            }
        }
    }
    
    // Build new route using A*
    vector<int> newRoute = {currentLocation};
    int currentPos = currentLocation;
    
    for (int nextLoc : remainingLocations) {
        auto [path, cost] = ShortestPathAlgorithms::aStar(graph, currentPos, nextLoc);
        
        if (!path.empty()) {
            for (size_t i = 1; i < path.size(); i++) {
                newRoute.push_back(path[i]);
            }
            currentPos = nextLoc;
        } else {
            cout << "  Warning: No path found to location " << nextLoc << endl;
        }
    }
    
    // Return to depot
    auto [pathToDepot, _] = ShortestPathAlgorithms::aStar(graph, currentPos, depot);
    if (!pathToDepot.empty()) {
        for (size_t i = 1; i < pathToDepot.size(); i++) {
            newRoute.push_back(pathToDepot[i]);
        }
    }
    
    routes[vehicleId] = newRoute;
    
    cout << "  New route: ";
    for (size_t i = 0; i < newRoute.size(); i++) {
        cout << newRoute[i];
        if (i < newRoute.size() - 1) cout << " -> ";
    }
    cout << endl;
    
    return newRoute;
}

DisasterReliefSolver::RouteDetails
DisasterReliefSolver::getRouteDetails(int vehicleId) const {
    RouteDetails details;
    details.vehicleId = vehicleId;
    details.totalDistance = 0.0;
    details.totalDemand = 0;
    details.locationsServed = 0;
    details.totalPriority = 0;
    
    auto it = routes.find(vehicleId);
    if (it == routes.end()) {
        return details;
    }
    
    details.route = it->second;
    
    if (details.route.size() <= 1) {
        return details;
    }
    
    // Calculate metrics
    for (size_t i = 0; i < details.route.size() - 1; i++) {
        int u = details.route[i];
        int v = details.route[i + 1];
        
        auto costOpt = graph.getEdgeCost(u, v, false);
        if (costOpt) {
            details.totalDistance += *costOpt;
        }
    }
    
    for (int nodeId : details.route) {
        if (nodeId != depot) {
            auto nodeOpt = graph.getNode(nodeId);
            if (nodeOpt) {
                details.totalDemand += nodeOpt->demand;
                details.totalPriority += nodeOpt->priority;
                details.locationsServed++;
            }
        }
    }
    
    return details;
}

void DisasterReliefSolver::printSolution() const {
    cout << "\n" << string(70, '=') << endl;
    cout << "DISASTER RELIEF OPTIMIZATION SOLUTION" << endl;
    cout << string(70, '=') << endl;
    
    for (const auto& vehicle : vehicles) {
        RouteDetails details = getRouteDetails(vehicle.id);
        
        cout << "\nVehicle " << vehicle.id << " (Capacity: " << vehicle.capacity << "):" << endl;
        cout << "  Route: ";
        for (size_t i = 0; i < details.route.size(); i++) {
            cout << details.route[i];
            if (i < details.route.size() - 1) cout << " -> ";
        }
        cout << endl;
        
        cout << "  Locations Served: " << details.locationsServed << endl;
        cout << "  Total Distance: " << fixed << setprecision(2) 
                  << details.totalDistance << endl;
        cout << "  Total Demand: " << details.totalDemand << "/" << vehicle.capacity << endl;
        cout << "  Total Priority: " << details.totalPriority << endl;
    }
    
    cout << "\n" << string(70, '=') << endl;
    cout << "Total Computation Time: " << fixed << setprecision(4) 
              << computationTime << " seconds" << endl;
    cout << "  - Dijkstra's Algorithm: " << fixed << setprecision(4) 
              << dijkstraTime << " seconds" << endl;
    cout << "  - Route Construction: " << fixed << setprecision(4) 
              << routeConstructionTime << " seconds" << endl;
    cout << "  - Route Optimization: " << fixed << setprecision(4) 
              << optimizationTime << " seconds" << endl;
    cout << string(70, '=') << endl;
}

void DisasterReliefSolver::exportSolution(const string& filename) const {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Cannot create file: " << filename << endl;
        return;
    }
    
    file << "{\n";
    file << "  \"routes\": {\n";
    
    bool firstVehicle = true;
    for (const auto& vehicle : vehicles) {
        if (!firstVehicle) file << ",\n";
        firstVehicle = false;
        
        RouteDetails details = getRouteDetails(vehicle.id);
        
        file << "    \"" << vehicle.id << "\": [";
        for (size_t i = 0; i < details.route.size(); i++) {
            file << details.route[i];
            if (i < details.route.size() - 1) file << ", ";
        }
        file << "]";
    }
    
    file << "\n  },\n";
    file << "  \"metrics\": {\n";
    file << "    \"computation_time\": " << computationTime << ",\n";
    file << "    \"dijkstra_time\": " << dijkstraTime << ",\n";
    file << "    \"route_construction_time\": " << routeConstructionTime << ",\n";
    file << "    \"optimization_time\": " << optimizationTime << "\n";
    file << "  }\n";
    file << "}\n";
    
    file.close();
    cout << "\nSolution exported to " << filename << endl;
}

} // namespace DisasterRelief

