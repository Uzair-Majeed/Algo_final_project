#ifndef TWO_OPT_ALGORITHM_H
#define TWO_OPT_ALGORITHM_H

#include "Graph.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>

using namespace std;

vector<int> twoOpt(const Graph& graph, const vector<int>& route) {

    if (route.size() <= 3){
        return route;
    } 

    bool improved = true;

    vector<int> bestRoute = route;

    while (improved) {
        improved = false;

        for (int i = 1; i < (int)bestRoute.size() - 2; i++) {

            for (int j = i + 1; j < (int)bestRoute.size() - 1; j++) {
            
                double currCost = 0.0;
                bool currValid = true;

                for (int k = 0; k < (int)bestRoute.size() - 1; k++) {
                    double cost = graph.getEdgeCost(bestRoute[k], bestRoute[k + 1]);
                    
                    if (cost < 0) { 
                        currValid = false; 
                        break; 
                    }

                    currCost += cost;
                }
                
                if (!currValid) continue;
                
                vector<int> newRoute = bestRoute;
                
                reverse(newRoute.begin() + i, newRoute.begin() + j + 1);
                
                double newCost = 0.0;
                bool valid = true;
                
                for (int k = 0; k < (int)newRoute.size() - 1; k++) {
                    double cost = graph.getEdgeCost(newRoute[k], newRoute[k + 1]);
                    
                    if (cost < 0) { 
                        valid = false; 
                        break; 
                    }
                    
                    newCost += cost;
                }
                
                if (valid && newCost < currCost) { 
                    bestRoute = newRoute; 
                    improved = true; 
                    break; 
                }
            }
            
            if (improved) break;
        }
    }
    return bestRoute;
}

#endif
