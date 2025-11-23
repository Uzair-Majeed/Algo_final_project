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

double cost(const Graph& graph, int x, int y) {
    return graph.getEdgeCost(x, y);
}

vector<int> twoOpt(const Graph& graph, const vector<int>& route) {

   int n = route.size();
    if (n <= 3) return route;

    vector<int> bestRoute = route;
    bool improved = true;

    while (improved) {
        improved = false;

        for (int i = 1; i < n - 2; i++) {
            for (int j = i + 1; j < n - 1; j++) {

                int A = bestRoute[i - 1];
                int B = bestRoute[i];
                int C = bestRoute[j];
                int D = bestRoute[j + 1];

                double before = cost(graph,A, B) + cost(graph,C, D);
                double after  = cost(graph,A, C) + cost(graph,B, D);

                if (before < 0 || after < 0) continue;

                if (after < before) {
                    reverse(bestRoute.begin() + i, bestRoute.begin() + j + 1);
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
