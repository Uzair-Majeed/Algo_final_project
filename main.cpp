/**
 * main.cpp - Disaster Response Routing & Resource Allocation System
 * 
 * This program:
 * 1. Loads graph data from input.json
 * 2. Allocates locations to vehicles using priority-based greedy algorithm
 * 3. Optimizes routes using 2-opt
 * 4. Computes costs using multi-objective function
 * 5. Outputs results to console and output.json
 * 
 * Compile: g++ -std=c++17 main.cpp -o disaster_relief
 * Run: ./disaster_relief
 */

#include "graph.h"
#include "algorithms.h"
#include "Allocation.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

// Simple JSON parser for loading input
// For production, consider using nlohmann/json library
Graph loadGraphFromJSON(const string& filename) {
    Graph graph;
    ifstream file(filename);
    
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    
    string content, line;
    while (getline(file, line)) {
        content += line;
    }
    file.close();
    
    // Parse nodes
    int nodesPos = (int)content.find("\"nodes\"");
    if (nodesPos != (int)string::npos) {
        int start = (int)content.find('[', nodesPos);
        int end = (int)content.find(']', start);
        string nodesStr = content.substr(start + 1, end - start - 1);
        
        int nodeStart = 0;
        while ((nodeStart = (int)nodesStr.find('{', nodeStart)) != (int)string::npos) {
            int nodeEnd = (int)nodesStr.find('}', nodeStart);
            string nodeStr = nodesStr.substr(nodeStart, nodeEnd - nodeStart + 1);
            
            int id = 0, demand = 0, priority = 0;
            
            // Extract id
            int idPos = (int)nodeStr.find("\"id\"");
            if (idPos != (int)string::npos) {
                int colon = (int)nodeStr.find(':', idPos);
                int comma = (int)nodeStr.find_first_of(",}", colon);
                id = stoi(nodeStr.substr(colon + 1, comma - colon - 1));
            }
            
            // Extract demand
            int demandPos = (int)nodeStr.find("\"demand\"");
            if (demandPos != (int)string::npos) {
                int colon = (int)nodeStr.find(':', demandPos);
                int comma = (int)nodeStr.find_first_of(",}", colon);
                demand = stoi(nodeStr.substr(colon + 1, comma - colon - 1));
            }
            
            // Extract priority
            int priorityPos = (int)nodeStr.find("\"priority\"");
            if (priorityPos != (int)string::npos) {
                int colon = (int)nodeStr.find(':', priorityPos);
                int comma = (int)nodeStr.find_first_of(",}", colon);
                priority = stoi(nodeStr.substr(colon + 1, comma - colon - 1));
            }
            
            graph.addNode(Node(id, demand, priority));
            nodeStart = nodeEnd + 1;
        }
    }
    
    // Parse edges
    int edgesPos = (int)content.find("\"edges\"");
    if (edgesPos != (int)string::npos) {
        int start = (int)content.find('[', edgesPos);
        int end = (int)content.find(']', start);
        string edgesStr = content.substr(start + 1, end - start - 1);
        
        int edgeStart = 0;
        while ((edgeStart = (int)edgesStr.find('{', edgeStart)) != (int)string::npos) {
            int edgeEnd = (int)edgesStr.find('}', edgeStart);
            string edgeStr = edgesStr.substr(edgeStart, edgeEnd - edgeStart + 1);
            
            int u = 0, v = 0;
            double cost = 0.0, reliability = 1.0;
            
            // Extract u
            int uPos = (int)edgeStr.find("\"u\"");
            if (uPos != (int)string::npos) {
                int colon = (int)edgeStr.find(':', uPos);
                int comma = (int)edgeStr.find(',', colon);
                u = stoi(edgeStr.substr(colon + 1, comma - colon - 1));
            }
            
            // Extract v
            int vPos = (int)edgeStr.find("\"v\"");
            if (vPos != (int)string::npos) {
                int colon = (int)edgeStr.find(':', vPos);
                int comma = (int)edgeStr.find(',', colon);
                v = stoi(edgeStr.substr(colon + 1, comma - colon - 1));
            }
            
            // Extract cost
            int costPos = (int)edgeStr.find("\"cost\"");
            if (costPos != (int)string::npos) {
                int colon = (int)edgeStr.find(':', costPos);
                int comma = (int)edgeStr.find(',', colon);
                cost = stod(edgeStr.substr(colon + 1, comma - colon - 1));
            }
            
            // Extract reliability
            int relPos = (int)edgeStr.find("\"reliability\"");
            if (relPos != (int)string::npos) {
                int colon = (int)edgeStr.find(':', relPos);
                int comma = (int)edgeStr.find_first_of(",}", colon);
                reliability = stod(edgeStr.substr(colon + 1, comma - colon - 1));
            }
            
            graph.addEdge(Edge(u, v, cost, reliability));
            edgeStart = edgeEnd + 1;
        }
    }
    
    return graph;
}

// Load vehicles from JSON file
vector<Vehicle> loadVehiclesFromJSON(const string& filename) {
    vector<Vehicle> vehicles;
    ifstream file(filename);
    
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    
    string content, line;
    while (getline(file, line)) {
        content += line;
    }
    file.close();
    
    // Parse vehicles
    int vehiclesPos = (int)content.find("\"vehicles\"");
    if (vehiclesPos != (int)string::npos) {
        int start = (int)content.find('[', vehiclesPos);
        int end = (int)content.find(']', start);
        string vehiclesStr = content.substr(start + 1, end - start - 1);
        
        int vehicleStart = 0;
        while ((vehicleStart = (int)vehiclesStr.find('{', vehicleStart)) != (int)string::npos) {
            int vehicleEnd = (int)vehiclesStr.find('}', vehicleStart);
            string vehicleStr = vehiclesStr.substr(vehicleStart, vehicleEnd - vehicleStart + 1);
            
            int id = 0, capacity = 0;
            
            // Extract id
            int idPos = (int)vehicleStr.find("\"id\"");
            if (idPos != (int)string::npos) {
                int colon = (int)vehicleStr.find(':', idPos);
                int comma = (int)vehicleStr.find_first_of(",}", colon);
                id = stoi(vehicleStr.substr(colon + 1, comma - colon - 1));
            }
            
            // Extract capacity
            int capPos = (int)vehicleStr.find("\"capacity\"");
            if (capPos != (int)string::npos) {
                int colon = (int)vehicleStr.find(':', capPos);
                int comma = (int)vehicleStr.find_first_of(",}", colon);
                capacity = stoi(vehicleStr.substr(colon + 1, comma - colon - 1));
            }
            
            vehicles.push_back(Vehicle(id, capacity));
            vehicleStart = vehicleEnd + 1;
        }
    }
    
    return vehicles;
}

// Save results to JSON file
void saveResultsToJSON(const string& filename,
                      const vector<Vehicle>& vehicles,
                      const Graph& graph) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot create file: " << filename << endl;
        return;
    }
    
    file << "{\n";
    file << "  \"routes\": {\n";
    
    // Write routes for each vehicle
    for (int i = 0; i < (int)vehicles.size(); i++) {
        const Vehicle& vehicle = vehicles[i];
        file << "    \"" << vehicle.id << "\": [";
        
        for (int j = 0; j < (int)vehicle.route.size(); j++) {
            file << vehicle.route[j];
            if (j < (int)vehicle.route.size() - 1) file << ", ";
        }
        file << "]";
        
        if (i < (int)vehicles.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  },\n";
    file << "  \"costs\": {\n";
    
    // Write cost breakdown for each vehicle
    for (int i = 0; i < (int)vehicles.size(); i++) {
        const Vehicle& vehicle = vehicles[i];
        RouteCost cost = calculateRouteCost(graph, vehicle.route, 
                                           vehicle.capacity, vehicle.currentLoad);
        
        file << "    \"" << vehicle.id << "\": {\n";
        file << "      \"total_time\": " << fixed << setprecision(2) 
             << cost.totalTime << ",\n";
        file << "      \"reliability_penalty\": " << cost.reliabilityPenalty << ",\n";
        file << "      \"idle_time\": " << cost.idleTime << ",\n";
        file << "      \"final_score\": " << cost.finalScore << "\n";
        file << "    }";
        
        if (i < (int)vehicles.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  }\n";
    file << "}\n";
    file.close();
}

int main() {
    try {
        cout << "========================================" << endl;
        cout << "Disaster Response Routing System" << endl;
        cout << "========================================" << endl << endl;
        
        // Load input from JSON file
        cout << "Loading input from input.json..." << endl;
        Graph graph = loadGraphFromJSON("input.json");
        vector<Vehicle> vehicles = loadVehiclesFromJSON("input.json");
        
        cout << "Graph loaded: " << graph.numNodes() << " nodes, " 
             << graph.numEdges() << " edges" << endl;
        cout << "Vehicles: " << vehicles.size() << endl << endl;
        
        // Step 1: Allocate vehicles using priority-based greedy algorithm
        cout << "Step 1: Allocating locations to vehicles..." << endl;
        vehicles = allocateVehicles(graph, vehicles);
        
        // Step 2: Optimize routes using 2-opt
        cout << "Step 2: Optimizing routes with 2-opt..." << endl;
        for (auto& vehicle : vehicles) {
            vehicle.route = twoOpt(graph, vehicle.route);
        }
        
        // Step 3: Calculate and display results
        cout << "Step 3: Calculating costs..." << endl << endl;
        
        cout << "========================================" << endl;
        cout << "SOLUTION" << endl;
        cout << "========================================" << endl << endl;
        
        // Print vehicle assignments
        cout << "Vehicle Assignments:" << endl;
        for (const auto& vehicle : vehicles) {
            cout << "Vehicle " << vehicle.id << ": ";
            bool first = true;
            for (int nodeId : vehicle.route) {
                if (nodeId != 0 || vehicle.route.size() == 1) {
                    if (!first) cout << " ";
                    cout << nodeId;
                    first = false;
                }
            }
            cout << endl;
        }
        cout << endl;
        
        // Print optimized routes
        cout << "Optimized Routes:" << endl;
        for (const auto& vehicle : vehicles) {
            cout << "Vehicle " << vehicle.id << " route: ";
            for (int i = 0; i < (int)vehicle.route.size(); i++) {
                cout << vehicle.route[i];
                if (i < (int)vehicle.route.size() - 1) cout << " -> ";
            }
            cout << endl;
        }
        cout << endl;
        
        // Print cost breakdown
        cout << "Cost Breakdown:" << endl;
        double totalScore = 0.0;
        for (const auto& vehicle : vehicles) {
            RouteCost cost = calculateRouteCost(graph, vehicle.route, 
                                               vehicle.capacity, vehicle.currentLoad);
            totalScore += cost.finalScore;
            
            cout << "Vehicle " << vehicle.id << ":" << endl;
            cout << "  Time: " << fixed << setprecision(2) 
                 << cost.totalTime << endl;
            cout << "  Reliability Penalty: " << setprecision(4) 
                 << cost.reliabilityPenalty << endl;
            cout << "  Idle: " << setprecision(2) 
                 << cost.idleTime << endl;
            cout << "  Final Score: " << setprecision(4) 
                 << cost.finalScore << endl;
        }
        cout << endl;
        cout << "Total Score: " << setprecision(4) << totalScore << endl;
        cout << endl;
        
        // Save results to JSON file
        cout << "Saving results to output.json..." << endl;
        saveResultsToJSON("output.json", vehicles, graph);
        
        cout << "Done!" << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
