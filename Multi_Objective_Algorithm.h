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
    
    double totalReliability = 1.0;

    //finding all edge reliabilities product and total time sume
    for (int i = 0; i < (int)route.size() - 1; i++) {
        int u = route[i];
        int v = route[i + 1];

        double EC = graph.getEdgeCost(u, v);

        double ER = graph.getEdgeReliability(u, v);

        if (EC >= 0) {
            cost.totalTime += EC;
            totalReliability *= ER;
        }
    }

    cost.reliabilityPenalty = 1.0 - totalReliability;

    cost.idleTime = vehicleCapacity - deliveredLoad;

    cost.finalScore = ALPHA * cost.totalTime + BETA * cost.reliabilityPenalty + GAMMA * cost.idleTime;

    return cost;
}

#endif