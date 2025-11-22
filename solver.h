/**
 * Main Solver for Disaster Relief Optimization - C++ Implementation
 * Integrates all algorithms into a cohesive system
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "graph.h"
#include "algorithms.h"
#include <unordered_map>
#include <vector>
#include <chrono>

using namespace std;

namespace DisasterRelief {

/**
 * Main solver for disaster relief optimization problem
 * Integrates multiple algorithmic approaches
 */
class DisasterReliefSolver {
private:
    Graph graph;
    vector<Vehicle> vehicles;
    int depot;
    double alpha;
    double beta;
    double gamma;
    
    // Solution storage
    unordered_map<int, vector<int>> routes;
    unordered_map<int, double> distancesFromDepot;
    unordered_map<int, int> parentsFromDepot;
    
    // Performance metrics
    double computationTime;
    double dijkstraTime;
    double routeConstructionTime;
    double optimizationTime;
    
    // Helper method for DP optimization
    void optimizeRoutesDP();
    
public:
    /**
     * Constructor
     */
    DisasterReliefSolver(
        const Graph& g,
        const vector<Vehicle>& v,
        int depot = 0,
        double alpha = 1.0,
        double beta = 0.5,
        double gamma = 0.3
    );
    
    /**
     * Main solving method - computes optimal routes
     */
    MultiObjectiveOptimization::SolutionMetrics solve(bool useOptimization = true);
    
    /**
     * Replan route for a vehicle when edges become blocked
     */
    vector<int> replanRoute(
        int vehicleId,
        int currentLocation,
        const vector<pair<int, int>>& blockedEdges = {}
    );
    
    /**
     * Get detailed information about a vehicle's route
     */
    struct RouteDetails {
        int vehicleId;
        vector<int> route;
        double totalDistance;
        int totalDemand;
        int locationsServed;
        int totalPriority;
    };
    
    RouteDetails getRouteDetails(int vehicleId) const;
    
    /**
     * Print solution in human-readable format
     */
    void printSolution() const;
    
    /**
     * Export solution to JSON file
     */
    void exportSolution(const string& filename) const;
    
    /**
     * Get routes
     */
    const unordered_map<int, vector<int>>& getRoutes() const {
        return routes;
    }
    
    /**
     * Get computation times
     */
    double getComputationTime() const { return computationTime; }
    double getDijkstraTime() const { return dijkstraTime; }
    double getRouteConstructionTime() const { return routeConstructionTime; }
    double getOptimizationTime() const { return optimizationTime; }
};

} // namespace DisasterRelief

#endif // SOLVER_H

