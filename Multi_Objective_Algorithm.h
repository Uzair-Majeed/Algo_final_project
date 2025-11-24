#ifndef MULTI_OBJECTIVE_ALGORITHM_H
#define MULTI_OBJECTIVE_ALGORITHM_H

#include "Graph.h"
#include "Greedy_Allocation.h"
#include <vector>
#include <cmath>
#include <algorithm> 

using namespace std;

const double ALPHA = 0.6; 
const double BETA = 0.2;  
const double GAMMA = 0.1; 

struct RouteCost {
    double totalTime;         
    double reliabilityPenalty; 
    double idleTime;          
    double finalScore;
    
    RouteCost() : totalTime(0.0), reliabilityPenalty(0.0), idleTime(0.0), finalScore(0.0) {}
};

RouteCost calculateRouteCost(const Graph& graph, const vector<int>& route, int vehicleCapacity, int deliveredLoad) {
    RouteCost cost;
    
    if (route.size() < 2){
        return cost;
    }
    
    double time = 0.0;
    double weightedSum = 0.0;
    double unreliability = 0.0;
    
    for (size_t i = 0; i < route.size() - 1; i++) {
        int u = route[i];
        int v = route[i + 1];

        double EC = graph.getEdgeCost(u, v); 
        double ER = graph.getEdgeReliability(u, v); 

        if (EC >= 0) {
            time += EC;
            int priority = graph.getNode(v)->priority; 
            weightedSum += (priority * time);
            unreliability += (1.0 - ER);
        }
    }

    cost.totalTime = weightedSum;           //  sum(pi * ti)
    cost.reliabilityPenalty = unreliability; // sum(1 - r)
    cost.idleTime = max(0, vehicleCapacity - deliveredLoad); // idle(k)

    // Multi Objective Function Calculation
    // alpha * sum(pi * ti) + beta * sum(1 - r) + gamma * idle(k)

    cost.finalScore = (ALPHA * cost.totalTime) + (BETA * cost.reliabilityPenalty) + (GAMMA * cost.idleTime);

    return cost;
}

#endif