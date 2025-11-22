# Question 1: Algorithm Design and Selection

## CLO 1: Fundamental Categories of Algorithmic Problems

### Problem Classification

This disaster relief optimization problem spans multiple algorithmic categories:

#### 1. **Graph-Related Problems**
Our disaster relief system is fundamentally a graph problem where:
- **Nodes (V)**: Represent disaster-affected locations
- **Edges (E)**: Represent roads/routes with weights (cost, reliability)
- **Problem Type**: Weighted, undirected graph with dynamic edge weights

#### 2. **Optimization Problems**
The problem involves multi-objective optimization:
- Minimize total response time
- Maximize delivery coverage
- Respect capacity and time constraints
- Prioritize high-severity zones

#### 3. **NP-Complete Problems**
This is a variant of the **Vehicle Routing Problem (VRP)**, which is NP-Complete:
- **Reduction from TSP**: Can be reduced to Traveling Salesman Problem
- **Complexity Class**: NP-Hard (no known polynomial-time exact solution)
- **Approach**: Use approximation algorithms and heuristics

#### 4. **Dynamic Programming Problems**
Subproblems include:
- Resource allocation with capacity constraints (Knapsack-like)
- Optimal subset selection for vehicle routes
- Memoization of repeated path computations

#### 5. **Greedy Problems**
Applicable for:
- Priority-based location selection
- Nearest-neighbor heuristics
- Initial solution construction

---

## Algorithm Selection for Each Task

### Task 1: **Shortest Path Computation**
**Algorithm: Dijkstra's Algorithm**
- **Purpose**: Find shortest paths from depot to all locations
- **Complexity**: O((V + E) log V) with priority queue
- **Justification**: Optimal for non-negative edge weights, provides shortest path tree

**Alternative: A* Algorithm**
- **Purpose**: Heuristic-guided path finding for real-time replanning
- **Complexity**: O(b^d) where b is branching factor, d is depth
- **Justification**: Faster than Dijkstra when good heuristics available (e.g., Euclidean distance)

### Task 2: **Route Construction and Optimization**
**Algorithm: Greedy Approach with Priority Sorting**
- **Purpose**: Initial route construction based on priority levels
- **Complexity**: O(V log V + E)
- **Justification**: Fast approximation, serves high-priority zones first

**Algorithm: Dynamic Programming for Subset Optimization**
- **Purpose**: Optimize location subset selection given capacity constraints
- **Complexity**: O(V × C) where C is capacity
- **Justification**: Exact solution for knapsack-like subproblems

### Task 3: **Vehicle Routing and Resource Allocation**
**Algorithm: Modified Clarke-Wright Savings Algorithm (Greedy)**
- **Purpose**: Assign locations to vehicles efficiently
- **Complexity**: O(V²)
- **Justification**: Well-established VRP heuristic with good approximation ratio

**Algorithm: Branch and Bound**
- **Purpose**: Explore solution space with pruning
- **Complexity**: O(2^V) worst case, much better with pruning
- **Justification**: Can find optimal or near-optimal solutions for moderate problem sizes

### Task 4: **Dynamic Replanning**
**Algorithm: Incremental Dijkstra / D* Lite**
- **Purpose**: Efficiently recompute paths when edges fail
- **Complexity**: O(E log V) for incremental updates
- **Justification**: Avoids complete recomputation, updates only affected paths

### Task 5: **Multi-Objective Optimization**
**Algorithm: Weighted Sum Approach**
- **Purpose**: Combine objectives (time, reliability, idle cost) into single objective
- **Complexity**: Same as underlying algorithm
- **Justification**: Tunable weights (α, β, γ) allow flexible prioritization

**Alternative: Pareto Optimization / NSGA-II (Genetic Algorithm)**
- **Purpose**: Find Pareto-optimal solutions
- **Complexity**: O(MN²) where M is objectives, N is population
- **Justification**: Provides multiple trade-off solutions

### Task 6: **Graph Representation**
**Data Structure: Adjacency List**
- **Purpose**: Store graph efficiently
- **Space Complexity**: O(V + E)
- **Justification**: Efficient for sparse graphs (typical in road networks)

**Alternative: Priority Queue (Min-Heap)**
- **Purpose**: Extract minimum cost nodes in Dijkstra
- **Complexity**: O(log V) per operation
- **Justification**: Essential for efficient shortest path algorithms

---

## Summary of Algorithm Pipeline

```
1. Graph Construction → Adjacency List
2. Priority Ranking → Sorting (O(V log V))
3. Shortest Paths → Dijkstra's Algorithm (O((V+E) log V))
4. Route Initialization → Greedy Priority-based Construction (O(V log V))
5. Route Optimization → Dynamic Programming + Branch & Bound
6. Vehicle Assignment → Clarke-Wright Savings / Greedy Assignment
7. Dynamic Updates → Incremental Dijkstra / A*
8. Multi-objective → Weighted Sum Optimization
```

---

## Complexity Classes

| Component | Problem Class | Algorithm Type | Complexity |
|-----------|---------------|----------------|------------|
| Shortest Path | P | Dijkstra | O((V+E) log V) |
| VRP | NP-Hard | Approximation/Heuristic | O(V²) to O(2^V) |
| Knapsack (capacity) | NP-Complete | Dynamic Programming | O(V × C) |
| Priority Scheduling | P | Greedy | O(V log V) |
| Graph Traversal | P | BFS/DFS | O(V + E) |

---

## Approximation and Heuristics

Since VRP is NP-Hard, we use:
1. **Greedy Heuristics**: Fast, 2-approximation for TSP variants
2. **Dynamic Programming**: Exact for small subproblems
3. **Branch and Bound**: Optimal with pruning for moderate sizes
4. **Metaheuristics**: Genetic algorithms for large-scale instances

**Approximation Guarantee**: For metric TSP (triangle inequality holds), greedy gives 2-approximation. Our problem relaxes some constraints, so we aim for practical efficiency over theoretical guarantees.

