#ifndef GREEDY_ALLOCATION_H
#define GREEDY_ALLOCATION_H

#include "Graph.h"
#include "Astar_Algorithm.h"
#include "Two_Opt_Algorithm.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;


struct Vehicle {
    int id;
    int capacity;
    int currentLoad;
    vector<int> route;

    Vehicle() : id(0), capacity(0), currentLoad(0) {}

    Vehicle(int id, int capacity) : id(id), capacity(capacity), currentLoad(0) {
        route.push_back(0);
    }

    bool canServe(int demand){
        return currentLoad + demand <= capacity; 
    }

    void addNode(int nodeId, int demand) {
        route.push_back(nodeId); 
        currentLoad += demand; 
    }
};

bool helperSort(const Graph& graph, int a, int b) {
    const Node* nodeA = graph.getNode(a);
    const Node* nodeB = graph.getNode(b);

    if (!nodeA || !nodeB) return false;

    return nodeA->priority > nodeB->priority;
}

vector<Vehicle> allocateVehicles(const Graph& graph, const vector<Vehicle>& vehicles) {
    
    vector<Vehicle> V = vehicles;
    
    auto N = graph.getAllNodeIds();
    
    N.erase(std::remove(N.begin(), N.end(), 0), N.end());

    // lets sort by priority
    sort(N.begin(), N.end(), [&](int a, int b){
            return helperSort(graph, a, b);
    });


    double sumTime = 0; // nanoseconds
    int number = 0;
    for (int nodeId : N){

        const Node* node = graph.getNode(nodeId); 

        if(!node) continue;

        int bestVehicle = -1; 
        double minCost = INF;

        for (int i = 0 ; i < (int) V.size() ; i++){

            Vehicle& curr = V[i];

            if (curr.canServe(node->demand)){

                int lastNode = curr.route.back();

                
                auto start = high_resolution_clock::now();
                auto path = astar(graph, lastNode, nodeId);
                auto end = high_resolution_clock::now();
            
                sumTime += duration_cast<nanoseconds>(end - start).count();
                number++;

                if (path.empty()) continue;

                double totalCost = 0.0; 
                bool ok = true;
                
                for (int j = 0; j + 1 < path.size() ; j++){

                    double c = graph.getEdgeCost(path[j],path[j + 1]);

                    if (c < 0) {
                        ok=false; 
                        break; 
                    }

                    totalCost += c;
                }
                
                if (!ok) continue;

                if (totalCost < minCost){
                    minCost = totalCost; 
                    bestVehicle = i; 
                }
            }
        }

        if (bestVehicle >= 0){
            V[bestVehicle].addNode(nodeId, node->demand);
        } 
    }

    //this will just add source node so that vehicle returns back
    for (auto& v: V){
        v.route.push_back(0);
    }

    if (number > 0) {
        double avgTime = sumTime / number;
        cout << "Average A* runtime: " << avgTime << " ns" << endl;
    }
    return V;
}


#endif