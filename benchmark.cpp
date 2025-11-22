/**
 * Benchmark Implementation
 */

#include "benchmark.h"
#include "solver.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <set>

using namespace std;

namespace DisasterRelief {

// ============================================================================
// Dataset Generator
// ============================================================================

Graph DatasetGenerator::generateRandomGraph(
    int numNodes,
    int numEdges,
    double gridSize,
    int maxPriority,
    int maxDemand
) {
    Graph graph;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> posDis(0.0, gridSize);
    uniform_int_distribution<> priorityDis(1, maxPriority);
    uniform_int_distribution<> demandDis(1, maxDemand);
    uniform_real_distribution<> reliabilityDis(0.7, 1.0);
    
    // Generate nodes with random positions
    for (int i = 0; i < numNodes; i++) {
        Node node(
            i,
            i > 0 ? demandDis(gen) : 0,      // Depot has 0 demand
            i > 0 ? priorityDis(gen) : 0,    // Depot has 0 priority
            posDis(gen),
            posDis(gen)
        );
        graph.addNode(node);
    }
    
    // Generate edges ensuring connectivity
    int edgesAdded = 0;
    set<pair<int, int>> edgeSet;
    
    // First, create a spanning tree to ensure connectivity
    set<int> unconnected;
    for (int i = 1; i < numNodes; i++) {
        unconnected.insert(i);
    }
    set<int> connected = {0};
    
    uniform_int_distribution<> nodeDis(0, numNodes - 1);
    
    while (!unconnected.empty() && edgesAdded < numEdges) {
        // Pick random node from connected set
        auto connectedIt = connected.begin();
        advance(connectedIt, nodeDis(gen) % connected.size());
        int u = *connectedIt;
        
        // Pick random node from unconnected set
        auto unconnectedIt = unconnected.begin();
        advance(unconnectedIt, nodeDis(gen) % unconnected.size());
        int v = *unconnectedIt;
        
        if (edgeSet.count({u, v}) || edgeSet.count({v, u})) {
            continue;
        }
        
        // Calculate cost based on Euclidean distance
        auto nodeU = graph.getNode(u);
        auto nodeV = graph.getNode(v);
        if (!nodeU || !nodeV) continue;
        
        double dx = nodeU->x - nodeV->x;
        double dy = nodeU->y - nodeV->y;
        double distance = sqrt(dx * dx + dy * dy);
        
        Edge edge(u, v, distance, reliabilityDis(gen));
        graph.addEdge(edge);
        edgeSet.insert({u, v});
        edgesAdded++;
        
        connected.insert(v);
        unconnected.erase(v);
    }
    
    // Add remaining edges randomly
    while (edgesAdded < numEdges) {
        int u = nodeDis(gen);
        int v = nodeDis(gen);
        
        if (u == v || edgeSet.count({u, v}) || edgeSet.count({v, u})) {
            continue;
        }
        
        auto nodeU = graph.getNode(u);
        auto nodeV = graph.getNode(v);
        if (!nodeU || !nodeV) continue;
        
        double dx = nodeU->x - nodeV->x;
        double dy = nodeU->y - nodeV->y;
        double distance = sqrt(dx * dx + dy * dy);
        
        Edge edge(u, v, distance, reliabilityDis(gen));
        graph.addEdge(edge);
        edgeSet.insert({u, v});
        edgesAdded++;
    }
    
    return graph;
}

vector<Vehicle> DatasetGenerator::generateVehicles(
    int numVehicles,
    int minCapacity,
    int maxCapacity
) {
    vector<Vehicle> vehicles;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> capacityDis(minCapacity, maxCapacity);
    
    for (int i = 1; i <= numVehicles; i++) {
        vehicles.emplace_back(i, capacityDis(gen));
    }
    
    return vehicles;
}

// ============================================================================
// Performance Benchmark
// ============================================================================

void PerformanceBenchmark::runBenchmark(
    const vector<BenchmarkConfig>& testConfigs,
    int iterations
) {
    cout << string(70, '=') << endl;
    cout << "PERFORMANCE BENCHMARK" << endl;
    cout << string(70, '=') << endl;
    
    for (const auto& config : testConfigs) {
        cout << "\nConfiguration: " << config.numNodes << " nodes, "
                  << config.numEdges << " edges, " 
                  << config.numVehicles << " vehicles" << endl;
        
        vector<double> computationTimes;
        vector<double> dijkstraTimes;
        vector<double> routeConstructionTimes;
        vector<double> optimizationTimes;
        vector<double> totalDistances;
        vector<int> locationsServed;
        vector<int> totalPriorities;
        
        for (int i = 0; i < iterations; i++) {
            cout << "  Iteration " << (i + 1) << "/" << iterations << "..." << endl;
            
            // Generate dataset
            Graph graph = DatasetGenerator::generateRandomGraph(
                config.numNodes,
                config.numEdges
            );
            vector<Vehicle> vehicles = DatasetGenerator::generateVehicles(
                config.numVehicles
            );
            
            // Run solver
            DisasterReliefSolver solver(graph, vehicles);
            auto solution = solver.solve(true);
            
            // Collect metrics
            computationTimes.push_back(solver.getComputationTime());
            dijkstraTimes.push_back(solver.getDijkstraTime());
            routeConstructionTimes.push_back(solver.getRouteConstructionTime());
            optimizationTimes.push_back(solver.getOptimizationTime());
            totalDistances.push_back(solution.totalDistance);
            locationsServed.push_back(solution.locationsServed);
            totalPriorities.push_back(solution.totalPriority);
        }
        
        // Calculate averages
        BenchmarkResult result;
        result.numNodes = config.numNodes;
        result.numEdges = config.numEdges;
        result.numVehicles = config.numVehicles;
        
        auto avg = [](const vector<double>& vec) {
            return accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
        };
        auto avgInt = [](const vector<int>& vec) {
            return accumulate(vec.begin(), vec.end(), 0) / (double)vec.size();
        };
        
        result.avgComputationTime = avg(computationTimes);
        result.avgDijkstraTime = avg(dijkstraTimes);
        result.avgRouteConstructionTime = avg(routeConstructionTimes);
        result.avgOptimizationTime = avg(optimizationTimes);
        result.avgTotalDistance = avg(totalDistances);
        result.avgLocationsServed = avgInt(locationsServed);
        result.avgTotalPriority = avgInt(totalPriorities);
        
        results.push_back(result);
        
        cout << "  Average Computation Time: " << fixed << setprecision(4)
                  << result.avgComputationTime << " seconds" << endl;
        cout << "  Average Locations Served: " << fixed << setprecision(2)
                  << result.avgLocationsServed << endl;
    }
}

void PerformanceBenchmark::printResults() const {
    cout << "\n" << string(70, '=') << endl;
    cout << "BENCHMARK RESULTS SUMMARY" << endl;
    cout << string(70, '=') << endl;
    
    cout << left << setw(8) << "Nodes"
              << setw(8) << "Edges"
              << setw(10) << "Vehicles"
              << setw(12) << "Time (s)"
              << setw(12) << "Locations" << endl;
    cout << string(70, '-') << endl;
    
    for (const auto& result : results) {
        cout << left << setw(8) << result.numNodes
                  << setw(8) << result.numEdges
                  << setw(10) << result.numVehicles
                  << setw(12) << fixed << setprecision(4) << result.avgComputationTime
                  << setw(12) << fixed << setprecision(2) << result.avgLocationsServed
                  << endl;
    }
    
    cout << string(70, '=') << endl;
}

void PerformanceBenchmark::exportResults(const string& filename) const {
    ofstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Cannot create file: " << filename << endl;
        return;
    }
    
    file << "[\n";
    for (size_t i = 0; i < results.size(); i++) {
        const auto& r = results[i];
        if (i > 0) file << ",\n";
        
        file << "  {\n";
        file << "    \"num_nodes\": " << r.numNodes << ",\n";
        file << "    \"num_edges\": " << r.numEdges << ",\n";
        file << "    \"num_vehicles\": " << r.numVehicles << ",\n";
        file << "    \"avg_computation_time\": " << r.avgComputationTime << ",\n";
        file << "    \"avg_dijkstra_time\": " << r.avgDijkstraTime << ",\n";
        file << "    \"avg_route_construction_time\": " << r.avgRouteConstructionTime << ",\n";
        file << "    \"avg_optimization_time\": " << r.avgOptimizationTime << ",\n";
        file << "    \"avg_total_distance\": " << r.avgTotalDistance << ",\n";
        file << "    \"avg_locations_served\": " << r.avgLocationsServed << ",\n";
        file << "    \"avg_total_priority\": " << r.avgTotalPriority << "\n";
        file << "  }";
    }
    file << "\n]\n";
    
    file.close();
    cout << "\nBenchmark results exported to " << filename << endl;
}

void PerformanceBenchmark::analyzeComplexity() const {
    cout << "\n" << string(70, '=') << endl;
    cout << "EMPIRICAL COMPLEXITY ANALYSIS" << endl;
    cout << string(70, '=') << endl;
    
    if (results.size() < 2) {
        cout << "Need at least 2 data points for analysis" << endl;
        return;
    }
    
    cout << "\nScaling Analysis:" << endl;
    for (size_t i = 1; i < results.size(); i++) {
        const auto& prev = results[i - 1];
        const auto& curr = results[i];
        
        double nodeRatio = (double)curr.numNodes / prev.numNodes;
        double edgeRatio = (double)curr.numEdges / prev.numEdges;
        double timeRatio = curr.avgComputationTime / prev.avgComputationTime;
        
        cout << "\nFrom (" << prev.numNodes << "N, " << prev.numEdges << "E) to ("
                  << curr.numNodes << "N, " << curr.numEdges << "E):" << endl;
        cout << "  Node increase: " << fixed << setprecision(2)
                  << nodeRatio << "x" << endl;
        cout << "  Edge increase: " << fixed << setprecision(2)
                  << edgeRatio << "x" << endl;
        cout << "  Time increase: " << fixed << setprecision(2)
                  << timeRatio << "x" << endl;
        
        // Estimate complexity
        if (timeRatio > 0 && nodeRatio > 1) {
            double logComplexity = log(timeRatio) / log(nodeRatio);
            cout << "  Empirical complexity: O(n^" << fixed << setprecision(2)
                      << logComplexity << ")" << endl;
        }
    }
}

} // namespace DisasterRelief

