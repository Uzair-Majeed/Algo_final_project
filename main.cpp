#include <chrono>
#include "Graph.h"
#include "Greedy_Allocation.h"
#include "Multi_Objective_Algorithm.h"
#include "Two_Opt_Algorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#pragma region FileHandling

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
#pragma endregion

int main() {
    vector<string> datasetFiles = {
        "datasets/proj.json",
        "datasets/input1.json",
        "datasets/input2.json",
        "datasets/input3.json",
        "datasets/input4.json",
        "datasets/input5.json",
        "datasets/input6.json"
    };

    for (const auto& filename : datasetFiles) {
        try {
            cout << "\n\n===================================================" << endl;
            cout << "Dataset: " << filename << endl;
            cout << "Disaster Response Dawooo Bus" << endl;
            cout << "===================================================" << endl << endl;

            cout << "Loading input from " << filename << "..." << endl;
            Graph graph = loadGraphFromJSON(filename);
            vector<Vehicle> vehicles = loadVehiclesFromJSON(filename);

            cout << "Graph loaded: " << graph.numNodes() << " nodes, "
                 << graph.numEdges() << " edges" << endl;
            cout << "Vehicles: " << vehicles.size() << endl << endl;

            
            cout << "===================================================" << endl;
            cout << "Starting Engine" << endl;
            cout << "===================================================" << endl << endl;

            
            cout << "\n===================================================" << endl;
            cout << "Step 1 : Allocating Routes to Vehicles..." << endl;
            cout << "===================================================" << endl << endl;
            
            // --- 1) Allocate Vehicles ---
            auto startA = high_resolution_clock::now();
            
            vehicles = allocateVehicles(graph, vehicles);
            
            auto endA = high_resolution_clock::now();
            
            double timeA = duration_cast<nanoseconds>(endA - startA).count();
            
            cout << "Greedy Allocation runtime: " << timeA << " ns" << endl;

            // --- 2) Run 2 opt route optimization---
            
            cout << "\n===================================================" << endl;
            cout << "Step 2 :Optimizing Routes Further..." << endl;
            cout << "===================================================" << endl << endl;
            
            auto startB = high_resolution_clock::now();
            
            for (auto& vehicle : vehicles) {
                vehicle.route = twoOpt(graph, vehicle.route);
            }
            
            auto endB = high_resolution_clock::now();
            
            double timeB = duration_cast<nanoseconds>(endB - startB).count();
            
            cout << "2-Opt total runtime: " << timeB << " ns" << endl;

            //3) calculate multi objected weighted score

            cout << "\n===================================================" << endl;
            cout << "Step 3 : Calculating Overall Results and Scores..." << endl;
            cout << "=====================================================" << endl << endl;
            
            double avgR = 0.0,pScore = 0.0,totalR = 0.0;
            int totalE = 0,totalC = 0, totalPNodes = 0,servedPNodes = 0;
            
            double timeC = 0.0;
            
            for (const auto& vehicle : vehicles) {
                
                auto startC = high_resolution_clock::now();
                
                RouteCost cost = calculateRouteCost(graph, vehicle.route,vehicle.capacity, vehicle.currentLoad);
                
                auto endC = high_resolution_clock::now();

                timeC += duration_cast<nanoseconds>(endC - startC).count();
                
                cout << "\nVehicle " << vehicle.id << " Route : ";
                
                for (int i = 0; i < vehicle.route.size(); i++) {
                    cout << vehicle.route[i];
                    if (i < vehicle.route.size() - 1) cout << " -> ";
                }
            
                cout << endl;

                cout << "Delivered Demand : " << vehicle.currentLoad << endl;
                cout << "Total Cost : " << fixed << setprecision(2) << cost.finalScore << endl;

                totalC += (int)cost.finalScore;

                // Reliability accumulation
    
                for (int i = 0; i < vehicle.route.size() - 1; i++) {
                    totalR += graph.getEdgeReliability(vehicle.route[i], vehicle.route[i+1]);
                    totalE++;
                }
    
                // Priority Score
                for (int nodeId : vehicle.route) {
                    if (nodeId == 0) continue; 
                    
                    Node* node = graph.getNode(nodeId);

                    if (node && node->priority > 0) {
                    totalPNodes++;

                    if (vehicle.currentLoad > 0) servedPNodes++;
                    }
                }
            }



            avgR = (totalE > 0) ? totalR / (double)totalE : 0.0;
            pScore = (totalPNodes > 0) ? (double)servedPNodes / totalPNodes : 1.0;


            cout << "\nTotal Combined Cost : " << fixed << setprecision(2) << totalC << endl;
            cout << "Average Reliability : " << fixed << setprecision(3) << avgR << endl;
            cout << "Priority Satisfaction Score : " << fixed << setprecision(2) << pScore << endl;
            
            
            cout << "\n\nAverage Multi Objective Weighted Scoring runtime: " << timeC / (double)vehicles.size() << " ns" << endl;

            saveResultsToJSON("output.json", vehicles, graph);
            
            cout << "\n===================================================" << endl;
            cout << "Thank you for using Dawoo Express..." << endl;
            cout << "===================================================" << endl << endl;

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}