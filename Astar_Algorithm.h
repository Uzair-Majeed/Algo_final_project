#ifndef ASTAR_ALGORITHM_H
#define ASTAR_ALGORITHM_H

#include "Graph.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>

using namespace std;

const double LAMBDA = 1.0;    
const double MU = 1.0;
const double INF = numeric_limits<double>::infinity();

double heuristic(const Graph& graph, int current, int target) {
    
    double minEdgeCost = INF;
    double priorityFactor = 0.0;
    double maxReliability = 0.0;

    for (auto& neighbor : graph.getNeighbors(current)) {
        minEdgeCost = min(minEdgeCost, neighbor.second.cost);
        maxReliability = max(maxReliability, neighbor.second.reliability);
    }
    
    if (minEdgeCost == INF) minEdgeCost = 0.0;

    const Node* node = graph.getNode(current);

    if(node){
        priorityFactor = node->priority;
    }

    double reliabilityFactor = 1.0 - maxReliability; 

    double h = minEdgeCost - LAMBDA * priorityFactor + MU * reliabilityFactor;
    
    return max(0.0, h); // heuristic must be non-negative
}

// A* search
vector<int> astar(const Graph& graph, int start, int target) {
    
    unordered_map<int, double> g; //old distance
    unordered_map<int, double> f; // new distance
    unordered_map<int, int> prev; //for backtracking
    unordered_map<int, bool> visited; //visited node arr

    for (int nodeId : graph.getAllNodeIds()) {
        g[nodeId] = INF;
        f[nodeId] = INF;
        visited[nodeId] = false;
    }

    g[start] = 0.0;
    f[start] = g[start] + heuristic(graph, start, target);

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    
    pq.push({f[start], start});

    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();

        if (visited[current]) continue;

        visited[current] = true;

        if (current == target) {
            vector<int> path;
            int node = target;

            //backtracking to construct path
            while (node != -1) {
                path.push_back(node);
                auto it = prev.find(node);

                if(it != prev.end()){
                    node = it->second;
                }
                else{
                    node = -1;
                }
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (auto& neighbor : graph.getNeighbors(current)) {

            int v = neighbor.first;
            double cost = neighbor.second.cost;

            //relaxation like dijstra
            if (g[current] + cost < g[v]) {
                prev[v] = current;
                g[v] = g[current] + cost;
                f[v] = g[v] + heuristic(graph, v, target);
                pq.push({f[v], v});
            }
        }
    }

    return {}; // no path found
}



#endif
