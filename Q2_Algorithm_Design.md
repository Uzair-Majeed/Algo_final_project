# Question 2: Algorithm Design

## CLO 4: Efficient Algorithmic Solutions using Design Paradigms

---

## Algorithm 1: Priority-Greedy Route Construction

### Design Paradigm: **Greedy Algorithm**

### Algorithm Description
Constructs initial routes by greedily selecting highest-priority, closest locations that fit vehicle capacity constraints.

### Pseudocode

```
ALGORITHM GreedyPriorityRouting(Graph G, Vehicles K, Depot d)
INPUT: 
    - G = (V, E): Weighted graph with nodes having demand d_i and priority p_i
    - K: Set of vehicles with capacity C_k
    - d: Depot node
OUTPUT: 
    - Routes: Assignment of nodes to vehicle routes

1. Compute shortest paths from depot d to all nodes using Dijkstra
2. Sort all nodes by priority (descending), then by distance (ascending)
3. Initialize empty route for each vehicle k ∈ K
4. FOR each vehicle k in K:
5.     remaining_capacity ← C_k
6.     current_location ← depot
7.     total_time ← 0
8.     WHILE unvisited high-priority nodes exist:
9.         best_node ← NULL
10.        best_score ← -∞
11.        FOR each unvisited node v:
12.            IF demand[v] ≤ remaining_capacity:
13.                score ← priority[v] / (distance[current_location][v] + 1)
14.                IF score > best_score:
15.                    best_score ← score
16.                    best_node ← v
17.        IF best_node is NULL:
18.            BREAK
19.        Add best_node to route[k]
20.        remaining_capacity ← remaining_capacity - demand[best_node]
21.        total_time ← total_time + distance[current_location][best_node]
22.        current_location ← best_node
23.        Mark best_node as visited
24.    Add return to depot to route[k]
25. RETURN Routes
```

### Time Complexity
- **Dijkstra**: O((V + E) log V)
- **Sorting**: O(V log V)
- **Greedy Selection**: O(K × V²) in worst case
- **Total**: O((V + E) log V + K × V²)

### Space Complexity
- O(V + E) for graph storage
- O(V) for distance arrays
- O(K × V) for routes

---

## Algorithm 2: Dynamic Programming for Capacity-Constrained Selection

### Design Paradigm: **Dynamic Programming**

### Algorithm Description
Given a vehicle with capacity C and a set of locations with demands and priorities, find the optimal subset that maximizes total priority while respecting capacity.

### Problem Formulation
```
Maximize: Σ(priority[i] × selected[i])
Subject to: Σ(demand[i] × selected[i]) ≤ C
```

This is a **0/1 Knapsack Problem** variant.

### Pseudocode

```
ALGORITHM DynamicCapacityOptimization(Locations L, Capacity C)
INPUT:
    - L: Set of locations with demand[i] and priority[i]
    - C: Vehicle capacity
OUTPUT:
    - Selected: Optimal subset of locations

1. n ← |L|
2. Create DP table dp[n+1][C+1] initialized to 0
3. Create selection table keep[n+1][C+1] initialized to FALSE
4. 
5. FOR i from 1 to n:
6.     FOR w from 0 to C:
7.         IF demand[i] > w:
8.             dp[i][w] ← dp[i-1][w]
9.         ELSE:
10.            include_value ← dp[i-1][w - demand[i]] + priority[i]
11.            exclude_value ← dp[i-1][w]
12.            IF include_value > exclude_value:
13.                dp[i][w] ← include_value
14.                keep[i][w] ← TRUE
15.            ELSE:
16.                dp[i][w] ← exclude_value
17.
18. // Backtrack to find selected items
19. Selected ← empty set
20. w ← C
21. FOR i from n down to 1:
22.     IF keep[i][w]:
23.         Add location i to Selected
24.         w ← w - demand[i]
25.
26. RETURN Selected
```

### Time Complexity
- O(n × C) where n = |L| and C is capacity

### Space Complexity
- O(n × C) for DP table
- Can be optimized to O(C) using rolling array

---

## Algorithm 3: Dijkstra with Reliability Weighting

### Design Paradigm: **Greedy (Shortest Path)**

### Algorithm Description
Modified Dijkstra that considers both travel time and reliability in edge weights.

### Modified Edge Weight
```
weight(u, v) = cost(u, v) × (2 - reliability(u, v))
```

This penalizes unreliable edges while maintaining shortest path properties.

### Pseudocode

```
ALGORITHM ReliableDijkstra(Graph G, Source s)
INPUT:
    - G = (V, E): Graph with cost(u,v) and reliability(u,v)
    - s: Source node
OUTPUT:
    - dist[]: Shortest weighted distances
    - parent[]: Predecessor for path reconstruction

1. FOR each vertex v in V:
2.     dist[v] ← ∞
3.     parent[v] ← NULL
4.     visited[v] ← FALSE
5. dist[s] ← 0
6. 
7. Create min-heap Q with all vertices, keyed by dist
8. 
9. WHILE Q is not empty:
10.    u ← ExtractMin(Q)
11.    visited[u] ← TRUE
12.    
13.    FOR each neighbor v of u:
14.        IF NOT visited[v]:
15.            // Compute reliability-weighted cost
16.            edge_weight ← cost(u, v) × (2 - reliability(u, v))
17.            alt ← dist[u] + edge_weight
18.            
19.            IF alt < dist[v]:
20.                dist[v] ← alt
21.                parent[v] ← u
22.                DecreaseKey(Q, v, alt)
23.
24. RETURN dist, parent
```

### Time Complexity
- O((V + E) log V) with binary heap
- O(V² + E) with Fibonacci heap

### Space Complexity
- O(V) for distance and parent arrays
- O(V) for priority queue

---

## Algorithm 4: A* Search for Real-Time Replanning

### Design Paradigm: **Heuristic Search (Informed Greedy)**

### Algorithm Description
Uses heuristic (Euclidean distance) to guide search toward goal, enabling faster replanning when routes are blocked.

### Pseudocode

```
ALGORITHM AStarReplanning(Graph G, start, goal, heuristic)
INPUT:
    - G = (V, E): Graph
    - start, goal: Source and destination nodes
    - heuristic(n): Estimated cost from n to goal
OUTPUT:
    - Path from start to goal

1. openSet ← {start}
2. cameFrom ← empty map
3. gScore[start] ← 0  // Cost from start to current
4. fScore[start] ← heuristic(start)  // Estimated total cost
5. 
6. WHILE openSet is not empty:
7.     current ← node in openSet with lowest fScore
8.     
9.     IF current = goal:
10.        RETURN ReconstructPath(cameFrom, current)
11.    
12.    Remove current from openSet
13.    
14.    FOR each neighbor of current:
15.        tentative_gScore ← gScore[current] + cost(current, neighbor)
16.        
17.        IF tentative_gScore < gScore[neighbor]:
18.            cameFrom[neighbor] ← current
19.            gScore[neighbor] ← tentative_gScore
20.            fScore[neighbor] ← gScore[neighbor] + heuristic(neighbor)
21.            
22.            IF neighbor not in openSet:
23.                Add neighbor to openSet
24.
25. RETURN FAILURE  // No path found
```

### Heuristic Function
For nodes with coordinates:
```
h(n) = √((x_n - x_goal)² + (y_n - y_goal)²)
```

### Time Complexity
- O(b^d) where b = branching factor, d = depth
- With good heuristic: O(E log V) (similar to Dijkstra)

### Space Complexity
- O(V) for open/closed sets

---

## Algorithm 5: Multi-Objective Optimization

### Design Paradigm: **Weighted Sum Method**

### Objective Function
```
Minimize: α × Σ(p_i × t_i) + β × Σ(1 - r(u,v)) + γ × Σ idle(k)
```

Where:
- α: Weight for priority-weighted time
- β: Weight for unreliability penalty
- γ: Weight for vehicle idle cost

### Algorithm Description
Combine all objectives into single score function used in route evaluation.

### Pseudocode

```
ALGORITHM MultiObjectiveScore(Route R, α, β, γ)
INPUT:
    - R: Proposed route
    - α, β, γ: Objective weights
OUTPUT:
    - Total score (lower is better)

1. priority_time ← 0
2. unreliability ← 0
3. idle_cost ← 0
4. 
5. FOR each location i in R:
6.     priority_time ← priority_time + priority[i] × arrival_time[i]
7. 
8. FOR each edge (u, v) in R:
9.     unreliability ← unreliability + (1 - reliability(u, v))
10.
11. FOR each vehicle k:
12.    IF capacity_used[k] < capacity[k]:
13.        idle_cost ← idle_cost + (capacity[k] - capacity_used[k])
14.
15. total_score ← α × priority_time + β × unreliability + γ × idle_cost
16. RETURN total_score
```

---

## Integrated System Algorithm

### Complete Pipeline

```
ALGORITHM DisasterReliefOptimization(G, K, depot)
INPUT:
    - G: Graph of disaster area
    - K: Set of vehicles
    - depot: Starting location
OUTPUT:
    - Optimal routes for all vehicles

1. // Phase 1: Preprocessing
2. Compute shortest paths from depot using ReliableDijkstra(G, depot)
3. Sort locations by priority (descending)
4. 
5. // Phase 2: Initial Route Construction
6. routes ← GreedyPriorityRouting(G, K, depot)
7. 
8. // Phase 3: Route Optimization
9. FOR each vehicle k in K:
10.    locations_in_route ← GetLocations(routes[k])
11.    optimal_subset ← DynamicCapacityOptimization(locations_in_route, capacity[k])
12.    routes[k] ← ReorderRoute(optimal_subset)
13.
14. // Phase 4: Multi-Objective Evaluation
15. best_score ← ∞
16. best_routes ← routes
17. FOR each weight combination (α, β, γ):
18.    score ← MultiObjectiveScore(routes, α, β, γ)
19.    IF score < best_score:
20.        best_score ← score
21.        best_routes ← routes
22.
23. // Phase 5: Dynamic Replanning (if needed)
24. IF route becomes blocked:
25.    blocked_vehicle ← GetAffectedVehicle()
26.    current_pos ← GetCurrentPosition(blocked_vehicle)
27.    remaining_locations ← GetRemainingLocations(blocked_vehicle)
28.    FOR each location in remaining_locations:
29.        new_path ← AStarReplanning(G, current_pos, location, heuristic)
30.        Update route
31.
32. RETURN best_routes
```

### Overall Complexity
- **Preprocessing**: O((V + E) log V)
- **Route Construction**: O(K × V²)
- **Optimization**: O(K × V × C)
- **Total**: O((V + E) log V + K × V² + K × V × C)

For reasonable K (< 100) and C (< 1000), this is practical for V up to 10,000.

---

## Correctness and Optimality

1. **Dijkstra**: Proven optimal for shortest paths
2. **Dynamic Programming**: Optimal for knapsack subproblems
3. **Greedy**: Provides good approximation (not always optimal)
4. **A***: Optimal if heuristic is admissible and consistent
5. **Overall System**: Near-optimal due to greedy components, but very practical

The system trades theoretical optimality for computational efficiency, which is necessary given the NP-Hard nature of the problem.

