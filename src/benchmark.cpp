#include "avl.hpp"
#include "bst.hpp"
#include "splay.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int element_count = 1001;
constexpr int search_count = 100000;
constexpr unsigned int random_seed = 42;

template <typename Tree>
void insert_values(Tree& tree, const std::vector<int>& values) {
    for (const int value : values) {
        tree.insert(value);
    }
}

template <typename Tree>
double calculate_average_depth(
    Tree& tree,
    const std::vector<int>& search_values
) {
    long long total_depth = 0;

    for (const int value : search_values) {
        total_depth += tree.find(value);
    }

    return static_cast<double>(total_depth) /
           static_cast<double>(search_values.size());
}

std::vector<int> create_ordered_values() {
    std::vector<int> values(element_count);
    std::iota(values.begin(), values.end(), 0);
    return values;
}

std::vector<int> create_random_values() {
    std::vector<int> values = create_ordered_values();
    std::mt19937 generator(random_seed);
    std::shuffle(values.begin(), values.end(), generator);
    return values;
}

std::vector<int> create_search_values() {
    std::vector<int> values;
    values.reserve(search_count);

    std::mt19937 generator(random_seed + 1);
    std::uniform_int_distribution<int> distribution(0, element_count - 1);

    for (int i = 0; i < search_count; ++i) {
        values.push_back(distribution(generator));
    }

    return values;
}

void print_result(
    const std::string& scenario,
    const std::string& tree_name,
    const double average_depth
) {
    std::cout << std::left << std::setw(20) << scenario
              << std::setw(10) << tree_name
              << std::fixed << std::setprecision(4)
              << average_depth << '\n';
}

void run_scenario(
    const std::string& scenario,
    const std::vector<int>& insertion_values,
    const std::vector<int>& search_values
) {
    BST<int> bst;
    AvlTree<int> avl;
    Splay<int> splay;

    insert_values(bst, insertion_values);
    insert_values(avl, insertion_values);
    insert_values(splay, insertion_values);

    print_result(
        scenario,
        "BST",
        calculate_average_depth(bst, search_values)
    );
    print_result(
        scenario,
        "AVL",
        calculate_average_depth(avl, search_values)
    );
    print_result(
        scenario,
        "Splay",
        calculate_average_depth(splay, search_values)
    );
}

}  // namespace

int main() {
    const std::vector<int> ordered_values = create_ordered_values();
    const std::vector<int> random_values = create_random_values();
    const std::vector<int> search_values = create_search_values();

    std::cout << "Tree Depth Benchmark\n";
    std::cout << "Elements: " << element_count << '\n';
    std::cout << "Searches: " << search_count << '\n';
    std::cout << "Random seed: " << random_seed << "\n\n";

    std::cout << std::left << std::setw(20) << "Insertion order"
              << std::setw(10) << "Tree"
              << "Average depth\n";
    std::cout << std::string(47, '-') << '\n';

    run_scenario(
        "Ordered",
        ordered_values,
        search_values
    );
    run_scenario(
        "Random",
        random_values,
        search_values
    );

    return 0;
}