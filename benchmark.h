/**
 * Benchmarking and Performance Analysis Module - C++ Implementation
 * Generates test datasets and measures algorithm performance
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "graph.h"
#include <vector>
#include <string>

using namespace std;

namespace DisasterRelief {

/**
 * Benchmark configuration
 */
struct BenchmarkConfig {
    int numNodes;
    int numEdges;
    int numVehicles;
};

/**
 * Benchmark result
 */
struct BenchmarkResult {
    int numNodes;
    int numEdges;
    int numVehicles;
    double avgComputationTime;
    double avgDijkstraTime;
    double avgRouteConstructionTime;
    double avgOptimizationTime;
    double avgTotalDistance;
    double avgLocationsServed;
    double avgTotalPriority;
};

/**
 * Dataset Generator
 */
class DatasetGenerator {
public:
    /**
     * Generate random graph for testing
     */
    static Graph generateRandomGraph(
        int numNodes,
        int numEdges,
        double gridSize = 100.0,
        int maxPriority = 5,
        int maxDemand = 10
    );
    
    /**
     * Generate random vehicles
     */
    static vector<Vehicle> generateVehicles(
        int numVehicles,
        int minCapacity = 5,
        int maxCapacity = 20
    );
};

/**
 * Performance Benchmark
 */
class PerformanceBenchmark {
private:
    vector<BenchmarkResult> results;
    
public:
    /**
     * Run benchmarks for multiple configurations
     */
    void runBenchmark(
        const vector<BenchmarkConfig>& testConfigs,
        int iterations = 3
    );
    
    /**
     * Print benchmark results in table format
     */
    void printResults() const;
    
    /**
     * Export results to JSON
     */
    void exportResults(const string& filename) const;
    
    /**
     * Analyze empirical time complexity
     */
    void analyzeComplexity() const;
    
    /**
     * Get results
     */
    const vector<BenchmarkResult>& getResults() const {
        return results;
    }
};

} // namespace DisasterRelief

#endif // BENCHMARK_H

