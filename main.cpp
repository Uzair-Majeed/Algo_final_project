/**
 * Main Entry Point for Disaster Relief Optimization System - C++ Implementation
 */

#include "graph.h"
#include "solver.h"
#include "benchmark.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace DisasterRelief;

void createSampleDataset(const string& filename = "sample_input.json") {
    ofstream file(filename);
    file << R"({
  "nodes": [
    {"id": 0, "demand": 0, "priority": 0, "x": 50.0, "y": 50.0},
    {"id": 1, "demand": 3, "priority": 5, "x": 30.0, "y": 70.0},
    {"id": 2, "demand": 2, "priority": 3, "x": 70.0, "y": 80.0},
    {"id": 3, "demand": 4, "priority": 4, "x": 60.0, "y": 40.0},
    {"id": 4, "demand": 1, "priority": 2, "x": 80.0, "y": 20.0}
  ],
  "edges": [
    {"u": 0, "v": 1, "cost": 4, "reliability": 0.9},
    {"u": 0, "v": 2, "cost": 6, "reliability": 0.8},
    {"u": 1, "v": 2, "cost": 2, "reliability": 0.7},
    {"u": 1, "v": 3, "cost": 5, "reliability": 0.95},
    {"u": 2, "v": 3, "cost": 3, "reliability": 0.85},
    {"u": 3, "v": 4, "cost": 4, "reliability": 0.9}
  ]
})";
    file.close();
    cout << "Sample dataset created: " << filename << endl;
}

void solveSampleProblem() {
    cout << "\n" << string(70, '=') << endl;
    cout << "SOLVING SAMPLE PROBLEM FROM PROJECT SPECIFICATION" << endl;
    cout << string(70, '=') << "\n" << endl;
    
    createSampleDataset("sample_input.json");
    Graph graph = Graph::fromJsonFile("sample_input.json");
    
    vector<Vehicle> vehicles = {Vehicle(1, 5), Vehicle(2, 6)};
    
    DisasterReliefSolver solver(graph, vehicles, 0, 1.0, 0.5, 0.3);
    auto solution = solver.solve(true);
    solver.printSolution();
    solver.exportSolution("sample_output.json");
}

void runScalabilityAnalysis() {
    cout << "\n" << string(70, '=') << endl;
    cout << "RUNNING SCALABILITY ANALYSIS" << endl;
    cout << string(70, '=') << "\n" << endl;
    
    vector<BenchmarkConfig> testConfigs = {
        {50, 100, 2}, {100, 200, 3}, {250, 500, 5}, {500, 1000, 5}
    };
    
    PerformanceBenchmark benchmark;
    benchmark.runBenchmark(testConfigs, 2);
    benchmark.printResults();
    benchmark.analyzeComplexity();
    benchmark.exportResults("benchmark_results.json");
}

void testDynamicReplanning() {
    cout << "\n" << string(70, '=') << endl;
    cout << "TESTING DYNAMIC REPLANNING" << endl;
    cout << string(70, '=') << "\n" << endl;
    
    Graph graph = DatasetGenerator::generateRandomGraph(20, 40, 100.0);
    vector<Vehicle> vehicles = {Vehicle(1, 15)};
    
    DisasterReliefSolver solver(graph, vehicles, 0);
    solver.solve(true);
    cout << "Initial Solution:" << endl;
    solver.printSolution();
    
    const auto& routes = solver.getRoutes();
    auto it = routes.find(1);
    if (it != routes.end() && it->second.size() >= 3) {
        const auto& route = it->second;
        pair<int, int> blockedEdge = {route[1], route[2]};
        cout << "\nBlocking edge: (" << blockedEdge.first << ", " << blockedEdge.second << ")" << endl;
        solver.replanRoute(1, route[1], {blockedEdge});
        cout << "\nReplanned Solution:" << endl;
        solver.printSolution();
    }
}

void interactiveMenu() {
    while (true) {
        cout << "\n" << string(70, '=') << endl;
        cout << "DISASTER RELIEF OPTIMIZATION SYSTEM (C++)" << endl;
        cout << string(70, '=') << endl;
        cout << "\nSelect an option:" << endl;
        cout << "1. Solve Sample Problem" << endl;
        cout << "2. Run Scalability Analysis" << endl;
        cout << "3. Test Dynamic Replanning" << endl;
        cout << "4. Run All Tests" << endl;
        cout << "5. Exit" << endl;
        cout << string(70, '=') << endl;
        cout << "\nEnter your choice (1-5): ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: solveSampleProblem(); break;
            case 2: runScalabilityAnalysis(); break;
            case 3: testDynamicReplanning(); break;
            case 4:
                cout << "\nRunning all tests...\n" << endl;
                solveSampleProblem();
                runScalabilityAnalysis();
                break;
            case 5:
                cout << "\nExiting..." << endl;
                return;
            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc > 1) {
            string command = argv[1];
            if (command == "sample") solveSampleProblem();
            else if (command == "benchmark") runScalabilityAnalysis();
            else if (command == "replan") testDynamicReplanning();
            else if (command == "all") {
                solveSampleProblem();
                runScalabilityAnalysis();
            }
            else {
                cerr << "Unknown command: " << command << endl;
                cerr << "Usage: " << argv[0] << " [sample|benchmark|replan|all]" << endl;
                return 1;
            }
        } else {
            interactiveMenu();
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
