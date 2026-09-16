# Self-Balancing Tree Benchmark

A reproducible C++ benchmark comparing the search depth of Binary Search Trees, AVL Trees, and Splay Trees under ordered and randomized insertion workloads.

The project demonstrates how insertion order affects tree structure and why balancing strategies matter for search performance.

## Key Results

The benchmark inserts 1,001 unique integers and performs 100,000 uniformly distributed searches using a fixed random seed.

Lower average depth indicates fewer tree levels visited during a search.

| Insertion order | BST | AVL | Splay |
|---|---:|---:|---:|
| Ordered | 500.4089 | 7.9877 | 11.4295 |
| Random | 10.5434 | 8.1581 | 11.4162 |

With ordered insertion, the regular BST degenerates into a linked-list-like structure, producing an average search depth of approximately 500.

The AVL Tree maintains logarithmic depth in both scenarios. The Splay Tree also avoids the severe degradation observed in the unbalanced BST and dynamically moves accessed elements toward the root.

## Implemented Data Structures

### Binary Search Tree

A standard unbalanced Binary Search Tree used as the baseline implementation.

Its performance depends heavily on insertion order:

- average case: `O(log n)`
- worst case: `O(n)`

### AVL Tree

A height-balanced Binary Search Tree that performs rotations after insertion.

AVL Trees guarantee:

- search: `O(log n)`
- insertion: `O(log n)`
- additional height bookkeeping

### Splay Tree

A self-adjusting Binary Search Tree that moves inserted or accessed elements to the root.

The implementation supports:

- zig rotations;
- zig-zig rotations;
- zig-zag rotations;
- splaying after insertion;
- splaying after lookup;
- promotion of the last accessed node after an unsuccessful lookup.

Splay Trees provide amortized `O(log n)` operations and can perform particularly well when some elements are accessed repeatedly.

## Methodology

The benchmark evaluates two insertion scenarios.

### Ordered insertion

Integers from `0` through `1000` are inserted in ascending order.

This represents a worst-case workload for an ordinary Binary Search Tree.

### Random insertion

The same 1,001 integers are shuffled using `std::mt19937` with a fixed seed before insertion.

Each tree receives exactly the same insertion sequence.

### Search workload

The benchmark generates 100,000 uniformly distributed search keys.

All tree implementations receive the same search sequence. A fixed seed makes every run reproducible.

```text
Element count: 1001
Search count: 100000
Random seed: 42
```

## Build and Run

### Requirements

- C++17-compatible compiler;
- CMake 3.20 or newer.

The project is tested with GCC and Clang on Linux through GitHub Actions.

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run the benchmark

```bash
./build/tree_benchmark
```

Example output:

```text
Tree Depth Benchmark
Elements: 1001
Searches: 100000
Random seed: 42

Insertion order     Tree      Average depth
-----------------------------------------------
Ordered             BST       500.4089
Ordered             AVL       7.9877
Ordered             Splay     11.4295
Random              BST       10.5434
Random              AVL       8.1581
Random              Splay     11.4162
```

## Testing

The project includes automated correctness tests for:

- BST lookup;
- AVL balancing under ordered insertion;
- Splay Tree promotion after insertion;
- Splay Tree promotion after lookup;
- preservation of values across repeated rotations;
- duplicate insertion handling.

Run the test suite with:

```bash
ctest --test-dir build --output-on-failure
```

Or run the test executable directly:

```bash
./build/tree_tests
```

## Continuous Integration

GitHub Actions automatically performs the following checks on every push and pull request targeting `main`:

- compilation with GCC;
- compilation with Clang;
- execution of the CTest suite;
- benchmark smoke test.

## Project Structure

```text
self-balancing-tree-benchmark/
├── .github/
│   └── workflows/
│       └── ci.yml
├── include/
│   ├── node.hpp
│   ├── bst.hpp
│   ├── avl.hpp
│   └── splay.hpp
├── results/
├── src/
│   └── benchmark.cpp
├── tests/
│   └── test_trees.cpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```

## Authorship and Attribution

The initial BST and AVL implementations were provided as part of a university data structures assignment.

My contribution includes:

- implementation of the Splay Tree;
- zig, zig-zig, and zig-zag rotations;
- parent-pointer management;
- benchmark design and implementation;
- deterministic workload generation;
- CMake project configuration;
- automated correctness tests;
- cross-compiler continuous integration;
- performance analysis and documentation.

This repository expands the original assignment into a reproducible and tested benchmarking project.

## Limitations

The current benchmark focuses on average search depth rather than wall-clock execution time.

It uses a uniform search distribution and a fixed dataset size. The results therefore demonstrate structural behavior but should not be interpreted as a complete real-world performance comparison.

## Roadmap

- [x] Implement BST, AVL, and Splay Tree comparison
- [x] Add reproducible random workloads
- [x] Add CMake build configuration
- [x] Add automated correctness tests
- [x] Add GCC and Clang continuous integration
- [ ] Add skewed and hot-key access distributions
- [ ] Compare multiple dataset sizes
- [ ] Measure execution time and rotation counts
- [ ] Export benchmark results to CSV
- [ ] Add performance visualizations
- [ ] Replace owning parent pointers with non-owning references

## License

This project is intended for educational and portfolio purposes.