# Self-Balancing Tree Benchmark

[![C++ CI](https://github.com/natusikse/self-balancing-tree-benchmark/actions/workflows/ci.yml/badge.svg)](https://github.com/natusikse/self-balancing-tree-benchmark/actions/workflows/ci.yml)

A reproducible C++ benchmark comparing Binary Search Trees, AVL Trees, and Splay Trees under ordered, randomized, and locality-focused access workloads.

The project demonstrates how insertion order and access patterns affect tree structure, average search depth, and the performance characteristics of self-balancing and self-adjusting trees.

## Key Results

The benchmark inserts 1,001 unique integers and performs 100,000 searches per workload using deterministic random-number generation.

Lower average depth indicates that fewer tree levels are visited during a search.

| Workload | BST | AVL | Splay |
|---|---:|---:|---:|
| Ordered insertion / uniform access | 500.4089 | 7.9877 | 11.4295 |
| Random insertion / uniform access | 10.5434 | 8.1581 | 11.4162 |
| Random insertion / hot-key access | 8.9734 | 8.0157 | **3.8613** |

With ordered insertion, the regular BST degenerates into a linked-list-like structure, producing an average search depth of approximately 500.

The AVL Tree maintains logarithmic depth regardless of insertion order or access distribution.

Under uniform access, the Splay Tree has a slightly greater average depth than the balanced AVL Tree. However, under the hot-key workload, its average depth decreases from `11.4162` to `3.8613`.

This is an improvement of approximately 66%. It demonstrates the main advantage of a self-adjusting tree: frequently accessed values are dynamically moved closer to the root.

## Implemented Data Structures

### Binary Search Tree

A standard unbalanced Binary Search Tree used as the baseline implementation.

Its performance depends heavily on insertion order:

- average search and insertion: `O(log n)`;
- worst-case search and insertion: `O(n)`;
- no automatic balancing.

### AVL Tree

A height-balanced Binary Search Tree that performs rotations after insertion.

AVL Trees provide:

- search: `O(log n)`;
- insertion: `O(log n)`;
- guaranteed logarithmic height;
- additional height bookkeeping and rebalancing operations.

### Splay Tree

A self-adjusting Binary Search Tree that moves inserted or accessed elements to the root.

The implementation supports:

- zig rotations;
- zig-zig rotations;
- zig-zag rotations;
- splaying after insertion;
- splaying after successful lookup;
- promotion of the last accessed node after an unsuccessful lookup.

Splay Trees provide amortized `O(log n)` operations. They can perform particularly well when a small subset of elements is accessed repeatedly.

## Benchmark Methodology

All workloads use the same dataset:

```text
Element count: 1001
Searches per workload: 100000
Random seed: 42
```

A fixed random seed makes the experiment reproducible. Running the benchmark multiple times produces the same insertion sequences, search sequences, and results.

### Ordered insertion

Integers from `0` through `1000` are inserted in ascending order.

This represents a worst-case workload for an ordinary Binary Search Tree because every new element is inserted as the right child of the previous element.

### Random insertion

The same 1,001 integers are shuffled using `std::mt19937` before insertion.

BST, AVL, and Splay Tree receive exactly the same insertion sequence.

### Uniform access

Search keys are uniformly distributed across the complete range from `0` through `1000`.

Every key has approximately the same probability of being accessed.

### Hot-key access

The hot-key workload models temporal locality:

- 90% of searches target 10 frequently accessed keys;
- 10% of searches target the complete key range;
- every tree receives the same search sequence;
- the workload uses a fixed seed for reproducibility.

This scenario highlights the adaptive behavior of the Splay Tree. Repeatedly accessed values are moved toward the root without storing explicit balance or frequency metadata.

## Build and Run

### Requirements

- a C++17-compatible compiler;
- CMake 3.20 or newer.

The project is automatically tested with GCC and Clang on Linux through GitHub Actions.

### Clone the repository

```bash
git clone https://github.com/natusikse/self-balancing-tree-benchmark.git
cd self-balancing-tree-benchmark
```

### Configure and build

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
Searches per workload: 100000
Random seed: 42
Hot-key workload: 90% of searches target 10 keys

Workload              Tree      Average depth
-------------------------------------------------
Ordered / uniform     BST       500.4089
Ordered / uniform     AVL       7.9877
Ordered / uniform     Splay     11.4295
Random / uniform      BST       10.5434
Random / uniform      AVL       8.1581
Random / uniform      Splay     11.4162
Random / hot-key      BST       8.9734
Random / hot-key      AVL       8.0157
Random / hot-key      Splay     3.8613
```

## Testing

The project includes automated correctness tests for:

- BST lookup;
- AVL balancing under ordered insertion;
- Splay Tree promotion after insertion;
- Splay Tree promotion after lookup;
- preservation of values across repeated rotations;
- duplicate insertion handling.

Run the complete test suite with:

```bash
ctest --test-dir build --output-on-failure
```

Alternatively, run the test executable directly:

```bash
./build/tree_tests
```

## Continuous Integration

GitHub Actions automatically performs the following checks on every push and pull request targeting `main`:

- configuration and compilation with GCC;
- configuration and compilation with Clang;
- execution of the correctness test suite through CTest;
- execution of the benchmark as a smoke test.

The workflow configuration is available in `.github/workflows/ci.yml`.

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

## Technologies

- C++17;
- CMake;
- CTest;
- GCC;
- Clang;
- GitHub Actions;
- standard C++ random-number facilities.

## Authorship and Attribution

The initial BST and AVL implementations were provided as part of a university data structures assignment.

My contribution includes:

- implementation of the Splay Tree;
- zig, zig-zig, and zig-zag rotations;
- parent-pointer management;
- benchmark design and implementation;
- deterministic workload generation;
- uniform and hot-key access scenarios;
- CMake project configuration;
- automated correctness tests;
- cross-compiler continuous integration;
- performance analysis and project documentation.

This repository expands the original assignment into a reproducible, tested, and documented benchmarking project.

## Limitations

The current benchmark focuses on average search depth rather than wall-clock execution time.

It uses uniform and synthetic hot-key access distributions with a fixed dataset size. Additional dataset sizes and real-world access traces would be needed for a broader performance evaluation.

The measured results describe these specific deterministic workloads and should not be interpreted as universal performance rankings for the three data structures.

## Roadmap

- [x] Compare BST, AVL, and Splay Tree behavior
- [x] Add reproducible random workloads
- [x] Add CMake build configuration
- [x] Add automated correctness tests
- [x] Add GCC and Clang continuous integration
- [x] Add a skewed hot-key access workload
- [ ] Compare multiple dataset sizes
- [ ] Measure execution time and rotation counts
- [ ] Export benchmark results to CSV
- [ ] Add performance visualizations
- [ ] Replace owning parent pointers with non-owning references

## Academic Context

This project was developed from a university data structures assignment and subsequently extended for educational and portfolio purposes.

The repository clearly distinguishes the provided starter implementations from my own implementation, experimental work, testing infrastructure, and documentation.