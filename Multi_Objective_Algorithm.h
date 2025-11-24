#ifndef MULTI_OBJECTIVE_ALGORITHM_H
#define MULTI_OBJECTIVE_ALGORITHM_H

#include "Graph.h"
#include "Greedy_Allocation.h"
#include <vector>
#include <cmath>

using namespace std;

const double ALPHA = 0.6;
const double BETA = 0.3;
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
    
    double totalReliabilityLog = 0.0; // sum of logs
    
    for (int i = 0; i < route.size() - 1; i++) {
        int u = route[i];
        int v = route[i + 1];

        double EC = graph.getEdgeCost(u, v);
        double ER = max(graph.getEdgeReliability(u, v), 1e-6); // clamp to >0

        if (EC >= 0) {
            cost.totalTime += EC;
            totalReliabilityLog += log(ER);  // log-sum
        }
    }

    double totalReliability = exp(totalReliabilityLog);  // convert back
    
    cost.reliabilityPenalty = 1.0 - totalReliability;

    cost.idleTime = max(0, vehicleCapacity - deliveredLoad);

    cost.finalScore = ALPHA * cost.totalTime + BETA * cost.reliabilityPenalty + GAMMA * cost.idleTime;

    return cost;
}

#endif