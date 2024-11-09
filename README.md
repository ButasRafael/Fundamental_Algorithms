# Fundamental Algorithms Performance Analysis

This repository features a collection of fundamental algorithms in computer science, implemented with a focus on performance measurement and analysis. Each algorithm has been rigorously tested and profiled to assess efficiency through key metrics, including comparisons, assignments, and in some cases, time complexity.

## Overview

The algorithms in this repository cover various categories:

- **Sorting Algorithms**: Direct sorting methods (Insertion Sort, Selection Sort, Bubble Sort, Binary Insertion Sort), Heapsort, Quicksort, Median QuickSort, Quicksort hybridization
- **Selection Algorithms**: Quickselect, Median of Medians
- **Data Structures**: Arrays, Linked Lists, Binary/Binary Search Trees, Stacks, Queues, Heaps, Hash Tables, Multi-way Trees, Red-Black Trees, Disjoint Sets
- **Graph Algorithms**: Breadth-First Search (BFS), Depth-First Search (DFS), Topological Sort, Tarjan's Algorithm, Kruskal's Algorithm
- **Order Statistics**: Dynamic Order Statistics
- **Tree Traversals**: Recursive and Iterative Binary Tree traversals and Pretty Prints

Each algorithm uses `profiler.h` for performance measurement, counting comparisons and assignments. For select algorithms, time complexity is also compared using `profiler.h`.


## Profiling with `profiler.h`

The `profiler.h` library is used across implementations to track the following metrics:

- **Comparisons**: Counts of comparison operations for evaluating algorithmic efficiency.
- **Assignments**: Counts of assignments, providing insights into memory usage and data movement.
- **Time Complexity**: Where applicable, time is also measured to give a holistic performance picture.

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/ButasRafael/Fundamental_Algorithms.git
2. Navigate to the desired algorithm's directory and build the code using your preferred compiler.
3. Run the program and observe the profiling output and/or the output in the console.
4. In each code you have a commented section in the beginning containing the description of the concepts coded and explanations of the profiling outputs/console outputs.
