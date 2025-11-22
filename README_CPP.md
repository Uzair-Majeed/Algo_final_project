# Disaster Relief Optimization System - C++ Implementation

**Course**: CS2009 - Design and Analysis of Algorithms  
**Institution**: National University of Computer and Emerging Sciences (NUCES)  
**Date**: November 24, 2025  
**Language**: C++17

---

## Overview

This is a complete C++ implementation of the disaster relief optimization system. The system uses advanced algorithms (Dijkstra, A*, Dynamic Programming, Greedy) to compute optimal routes for rescue vehicles in disaster-struck areas.

### Key Features

- ✅ **Pure C++ Implementation**: Uses STL containers and algorithms
- ✅ **High Performance**: Optimized with O2 compiler flags
- ✅ **Modern C++17**: Uses std::optional, structured bindings, etc.
- ✅ **Modular Design**: Clean separation of concerns
- ✅ **Easy Compilation**: Makefile and CMake support
- ✅ **No External Dependencies**: Uses only C++ standard library

---

## Project Structure

```
Algo_Project/
├── README_CPP.md              # This file
├── Makefile                   # Build system
├── CMakeLists.txt             # Alternative build system (CMake)
│
├── graph.h                    # Graph data structures (header)
├── graph.cpp                  # Graph implementation
├── algorithms.h               # Core algorithms (header)
├── algorithms.cpp             # Algorithm implementations
├── solver.h                   # Main solver (header)
├── solver.cpp                 # Solver implementation
├── benchmark.h                # Benchmarking (header)
├── benchmark.cpp              # Benchmark implementation
├── main.cpp                   # Main entry point
│
├── sample_input.json          # Sample problem input (generated)
├── sample_output.json         # Sample problem output (generated)
└── benchmark_results.json     # Benchmark results (generated)
```

---

## Compilation

### Method 1: Using Make (Recommended)

```bash
# Build the project
make

# Build with debug symbols
make debug

# Clean build artifacts
make clean
```

### Method 2: Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
make

# Run
./disaster_relief
```

### Method 3: Manual Compilation

```bash
# Compile with g++
g++ -std=c++17 -O2 -Wall -Wextra -o disaster_relief \
    main.cpp graph.cpp algorithms.cpp solver.cpp benchmark.cpp

# Run
./disaster_relief
```

### Method 4: Using Clang

```bash
# Compile with clang++
clang++ -std=c++17 -O2 -Wall -Wextra -o disaster_relief \
    main.cpp graph.cpp algorithms.cpp solver.cpp benchmark.cpp

# Run
./disaster_relief
```

---

## Requirements

### Minimum Requirements
- **Compiler**: g++ 7.0+ or clang++ 5.0+ (C++17 support required)
- **Operating System**: Linux, macOS, or Windows (with MinGW/MSYS2)
- **RAM**: 1 GB minimum
- **Disk Space**: 50 MB

### Recommended
- **Compiler**: g++ 10.0+ or clang++ 10.0+
- **Operating System**: Linux or macOS
- **RAM**: 4 GB
- **CPU**: Multi-core processor for better performance

---

## Usage

### Method 1: Interactive Menu

```bash
./disaster_relief
```

This launches an interactive menu with options:
1. Solve Sample Problem
2. Run Scalability Analysis
3. Test Dynamic Replanning
4. Generate Large Dataset
5. Run All Tests
6. Exit

### Method 2: Command Line Arguments

```bash
# Solve sample problem
./disaster_relief sample

# Run benchmarks
./disaster_relief benchmark

# Test dynamic replanning
./disaster_relief replan

# Generate large dataset
./disaster_relief generate

# Run all tests
./disaster_relief all
```

### Method 3: Using Make Targets

```bash
# Run sample problem
make run-sample

# Run benchmarks
make run-benchmark

# Run all tests
make run-all

# Run interactive mode
make run
```

---

## Code Structure

### Namespace Organization

All code is organized under the `DisasterRelief` namespace:

```cpp
namespace DisasterRelief {
    // Graph structures
    struct Node { ... };
    struct Edge { ... };
    struct Vehicle { ... };
    class Graph { ... };
    
    // Algorithms
    class ShortestPathAlgorithms { ... };
    class DynamicProgramming { ... };
    class GreedyAlgorithms { ... };
    class MultiObjectiveOptimization { ... };
    
    // Solver
    class DisasterReliefSolver { ... };
    
    // Benchmarking
    class DatasetGenerator { ... };
    class PerformanceBenchmark { ... };
}
```

### Class Hierarchy

```
Graph
├── Node (struct)
├── Edge (struct)
└── Neighbor (struct)

Vehicle (struct)

Algorithms (static classes)
├── ShortestPathAlgorithms
│   ├── dijkstra()
│   ├── aStar()
│   └── reconstructPath()
├── DynamicProgramming
│   └── knapsackCapacityOptimization()
├── GreedyAlgorithms
│   ├── priorityGreedyRouting()
│   └── nearestNeighborTSP()
└── MultiObjectiveOptimization
    ├── calculateRouteScore()
    └── evaluateSolution()

DisasterReliefSolver
├── solve()
├── replanRoute()
├── printSolution()
└── exportSolution()

Benchmarking
├── DatasetGenerator
│   ├── generateRandomGraph()
│   └── generateVehicles()
└── PerformanceBenchmark
    ├── runBenchmark()
    ├── printResults()
    └── analyzeComplexity()
```

---

## Algorithms Implemented

### 1. Dijkstra's Algorithm (`algorithms.cpp`)

```cpp
std::pair<std::unordered_map<int, double>, std::unordered_map<int, int>>
ShortestPathAlgorithms::dijkstra(const Graph& graph, int source, bool useReliability);
```

- **Time Complexity**: O((V + E) log V)
- **Space Complexity**: O(V + E)
- **Implementation**: Uses `std::priority_queue` for efficient min-heap operations

### 2. A* Search (`algorithms.cpp`)

```cpp
std::pair<std::vector<int>, double>
ShortestPathAlgorithms::aStar(const Graph& graph, int start, int goal);
```

- **Time Complexity**: O(E log V)
- **Space Complexity**: O(V)
- **Heuristic**: Euclidean distance

### 3. Dynamic Programming - Knapsack (`algorithms.cpp`)

```cpp
std::pair<std::vector<int>, int>
DynamicProgramming::knapsackCapacityOptimization(
    const std::vector<int>& locations, const Graph& graph, int capacity
);
```

- **Time Complexity**: O(n × C)
- **Space Complexity**: O(n × C)
- **Approach**: 0/1 Knapsack with backtracking

### 4. Greedy Priority Routing (`algorithms.cpp`)

```cpp
std::unordered_map<int, std::vector<int>>
GreedyAlgorithms::priorityGreedyRouting(
    const Graph& graph, std::vector<Vehicle>& vehicles, int depot
);
```

- **Time Complexity**: O((V + E) log V + K × V²)
- **Space Complexity**: O(K × V)
- **Strategy**: Priority-based location selection with distance weighting

---

## STL Containers Used

### Data Structures

1. **`std::vector`**: Dynamic arrays for routes, locations, edges
2. **`std::unordered_map`**: Hash maps for distances, parents, adjacency lists
3. **`std::unordered_set`**: Hash sets for visited nodes, tracking
4. **`std::priority_queue`**: Min-heap for Dijkstra and A*
5. **`std::pair`**: Return multiple values from functions
6. **`std::optional`**: Represent nullable values (C++17)
7. **`std::set`**: Ordered sets for edge tracking in graph generation

### Algorithms

1. **`std::sort`**: Sorting locations by priority
2. **`std::find`**: Finding elements in routes
3. **`std::remove_if`**: Removing edges/neighbors
4. **`std::accumulate`**: Computing averages in benchmarks
5. **`std::reverse`**: Path reconstruction

### Modern C++ Features

1. **Structured bindings** (C++17): `auto [distances, parents] = dijkstra(...)`
2. **`std::optional`** (C++17): Safe nullable node/edge access
3. **Range-based for loops**: Clean iteration
4. **Lambda functions**: Custom sorting, filtering
5. **Smart pointers**: (Not needed, but can be added)
6. **Move semantics**: Efficient resource management

---

## Performance Characteristics

### Compilation Time
- **Debug build**: ~2-3 seconds
- **Release build**: ~3-5 seconds

### Runtime Performance (Sample Problem - 5 nodes)
- **Total Time**: < 0.001 seconds
- **Dijkstra**: < 0.0001 seconds
- **Route Construction**: < 0.0001 seconds
- **Route Optimization**: < 0.0001 seconds

### Scalability (1000 nodes, 2000 edges)
- **Expected Time**: < 5 seconds
- **Memory Usage**: < 10 MB

### Comparison with Python Implementation
- **Speed**: ~10-50x faster than Python
- **Memory**: ~2-3x less memory usage
- **Startup**: ~100x faster (no interpreter)

---

## Sample Output

```
======================================================================
SOLVING SAMPLE PROBLEM FROM PROJECT SPECIFICATION
======================================================================

Sample dataset created: sample_input.json
Phase 1: Computing shortest paths from depot...
  Completed in 0.0000 seconds

Phase 2: Constructing initial routes (Greedy)...
  Completed in 0.0001 seconds

Phase 3: Optimizing routes (Dynamic Programming)...
  Completed in 0.0000 seconds

Phase 4: Evaluating solution...

======================================================================
DISASTER RELIEF OPTIMIZATION SOLUTION
======================================================================

Vehicle 1 (Capacity: 5):
  Route: 0 -> 1 -> 2 -> 0
  Locations Served: 2
  Total Distance: 12.00
  Total Demand: 5/5
  Total Priority: 8

Vehicle 2 (Capacity: 6):
  Route: 0 -> 3 -> 4 -> 0
  Locations Served: 2
  Total Distance: 4.00
  Total Demand: 5/6
  Total Priority: 6

======================================================================
Total Computation Time: 0.0002 seconds
  - Dijkstra's Algorithm: 0.0000 seconds
  - Route Construction: 0.0001 seconds
  - Route Optimization: 0.0000 seconds
======================================================================

Solution exported to sample_output.json
```

---

## Input/Output Format

### Input JSON Format

```json
{
  "nodes": [
    {"id": 0, "demand": 0, "priority": 0, "x": 50.0, "y": 50.0},
    {"id": 1, "demand": 3, "priority": 5, "x": 30.0, "y": 70.0}
  ],
  "edges": [
    {"u": 0, "v": 1, "cost": 4, "reliability": 0.9}
  ]
}
```

### Output JSON Format

```json
{
  "routes": {
    "1": [0, 1, 2, 0],
    "2": [0, 3, 4, 0]
  },
  "metrics": {
    "computation_time": 0.0002,
    "dijkstra_time": 0.0000,
    "route_construction_time": 0.0001,
    "optimization_time": 0.0000
  }
}
```

---

## Troubleshooting

### Compilation Errors

**Error**: `error: 'optional' is not a member of 'std'`
- **Solution**: Use C++17: `g++ -std=c++17 ...`

**Error**: `undefined reference to ...`
- **Solution**: Make sure all .cpp files are included in compilation

### Runtime Errors

**Error**: `Cannot open file: sample_input.json`
- **Solution**: Run from the correct directory or use `./disaster_relief sample` to auto-generate

**Error**: `Segmentation fault`
- **Solution**: Compile with debug symbols: `make debug` and use `gdb ./disaster_relief`

### Performance Issues

**Problem**: Slow execution on large datasets
- **Solution**: Use optimization flags: `-O2` or `-O3`
- **Solution**: Reduce problem size or increase timeout

---

## Advanced Features

### Custom Compilation Flags

```bash
# Maximum optimization
g++ -std=c++17 -O3 -march=native -o disaster_relief *.cpp

# Debug with sanitizers
g++ -std=c++17 -g -fsanitize=address -fsanitize=undefined -o disaster_relief *.cpp

# Profile-guided optimization
g++ -std=c++17 -O2 -fprofile-generate -o disaster_relief *.cpp
./disaster_relief benchmark
g++ -std=c++17 -O2 -fprofile-use -o disaster_relief *.cpp
```

### Memory Profiling

```bash
# Using Valgrind
valgrind --leak-check=full ./disaster_relief sample

# Using Heaptrack
heaptrack ./disaster_relief benchmark
```

### Static Analysis

```bash
# Using cppcheck
make analyze

# Using clang-tidy
clang-tidy *.cpp -- -std=c++17
```

---

## Development

### Adding New Features

1. **New Algorithm**: Add to `algorithms.h/cpp`
2. **New Metric**: Modify `MultiObjectiveOptimization`
3. **New Benchmark**: Extend `PerformanceBenchmark`

### Code Style

- **Indentation**: 4 spaces
- **Naming**: CamelCase for classes, camelCase for functions/variables
- **Comments**: Doxygen-style for public APIs
- **Line Length**: 100 characters maximum

### Testing

```bash
# Run all tests
make run-all

# Run specific test
./disaster_relief sample
./disaster_relief benchmark
./disaster_relief replan
```

---

## FAQ

**Q: Why C++ over Python?**  
A: C++ provides 10-50x better performance, crucial for large-scale problems (10,000+ nodes).

**Q: Can I use C++11/14 instead of C++17?**  
A: Partially. Remove `std::optional` and structured bindings. Not recommended.

**Q: How do I add more vehicles?**  
A: Edit the JSON input or modify `main.cpp` to create more `Vehicle` objects.

**Q: Can I compile on Windows?**  
A: Yes, use MinGW or MSYS2. Or use WSL (Windows Subsystem for Linux).

**Q: Is thread-safety supported?**  
A: No, current implementation is single-threaded. Can be added with OpenMP or std::thread.

---

## References

### C++ Resources
- [C++ Reference](https://en.cppreference.com/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)

### Algorithm Resources
- See main `README.md` for algorithm references

---

## Contact

For questions or issues with the C++ implementation:
- **Course**: CS2009 - Design and Analysis of Algorithms
- **Instructor**: Zeshan Khan
- **Institution**: NUCES Islamabad

---

## License

This project is submitted as coursework for CS2009. All rights reserved.

---

**Last Updated**: November 2025  
**Version**: 1.0 (C++ Implementation)  
**Status**: Complete ✅

