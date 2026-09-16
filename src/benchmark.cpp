#include "avl.hpp"
#include "bst.hpp"
#include "splay.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <system_error>
#include <vector>

namespace {

constexpr int element_count = 1001;
constexpr int search_count = 100000;
constexpr int hot_key_count = 10;
constexpr double hot_key_probability = 0.90;
constexpr unsigned int random_seed = 42;

const std::filesystem::path results_directory = "results";
const std::filesystem::path results_file =
    results_directory / "benchmark_results.csv";

struct BenchmarkResult {
    std::string workload;
    std::string tree_name;
    double average_depth;
};

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

std::vector<int> create_uniform_search_values() {
    std::vector<int> values;
    values.reserve(search_count);

    std::mt19937 generator(random_seed + 1);
    std::uniform_int_distribution<int> distribution(
        0,
        element_count - 1
    );

    for (int i = 0; i < search_count; ++i) {
        values.push_back(distribution(generator));
    }

    return values;
}

std::vector<int> create_hot_key_search_values() {
    std::vector<int> values;
    values.reserve(search_count);

    std::mt19937 generator(random_seed + 2);
    std::bernoulli_distribution choose_hot_key(hot_key_probability);

    std::uniform_int_distribution<int> hot_key_distribution(
        0,
        hot_key_count - 1
    );

    std::uniform_int_distribution<int> all_key_distribution(
        0,
        element_count - 1
    );

    for (int i = 0; i < search_count; ++i) {
        if (choose_hot_key(generator)) {
            values.push_back(hot_key_distribution(generator));
        } else {
            values.push_back(all_key_distribution(generator));
        }
    }

    return values;
}

std::vector<BenchmarkResult> run_scenario(
    const std::string& workload,
    const std::vector<int>& insertion_values,
    const std::vector<int>& search_values
) {
    BST<int> bst;
    AvlTree<int> avl;
    Splay<int> splay;

    insert_values(bst, insertion_values);
    insert_values(avl, insertion_values);
    insert_values(splay, insertion_values);

    const double bst_depth =
        calculate_average_depth(bst, search_values);

    const double avl_depth =
        calculate_average_depth(avl, search_values);

    const double splay_depth =
        calculate_average_depth(splay, search_values);

    return {
        {workload, "BST", bst_depth},
        {workload, "AVL", avl_depth},
        {workload, "Splay", splay_depth},
    };
}

void append_results(
    std::vector<BenchmarkResult>& destination,
    const std::vector<BenchmarkResult>& source
) {
    destination.insert(
        destination.end(),
        source.begin(),
        source.end()
    );
}

void print_results(const std::vector<BenchmarkResult>& results) {
    std::cout << std::left << std::setw(22) << "Workload"
              << std::setw(10) << "Tree"
              << "Average depth\n";

    std::cout << std::string(49, '-') << '\n';

    for (const BenchmarkResult& result : results) {
        std::cout << std::left << std::setw(22) << result.workload
                  << std::setw(10) << result.tree_name
                  << std::fixed << std::setprecision(4)
                  << result.average_depth << '\n';
    }
}

bool write_results_csv(
    const std::vector<BenchmarkResult>& results
) {
    std::error_code error;
    std::filesystem::create_directories(results_directory, error);

    if (error) {
        std::cerr << "Failed to create results directory: "
                  << error.message() << '\n';
        return false;
    }

    std::ofstream output(results_file);

    if (!output) {
        std::cerr << "Failed to open " << results_file << '\n';
        return false;
    }

    output << "workload,tree,average_depth\n";
    output << std::fixed << std::setprecision(4);

    for (const BenchmarkResult& result : results) {
        output << result.workload << ','
               << result.tree_name << ','
               << result.average_depth << '\n';
    }

    if (!output) {
        std::cerr << "Failed while writing " << results_file << '\n';
        return false;
    }

    return true;
}

}  // namespace

int main() {
    const std::vector<int> ordered_values = create_ordered_values();
    const std::vector<int> random_values = create_random_values();

    const std::vector<int> uniform_search_values =
        create_uniform_search_values();

    const std::vector<int> hot_key_search_values =
        create_hot_key_search_values();

    std::vector<BenchmarkResult> results;

    append_results(
        results,
        run_scenario(
            "Ordered / uniform",
            ordered_values,
            uniform_search_values
        )
    );

    append_results(
        results,
        run_scenario(
            "Random / uniform",
            random_values,
            uniform_search_values
        )
    );

    append_results(
        results,
        run_scenario(
            "Random / hot-key",
            random_values,
            hot_key_search_values
        )
    );

    std::cout << "Tree Depth Benchmark\n";
    std::cout << "Elements: " << element_count << '\n';
    std::cout << "Searches per workload: " << search_count << '\n';
    std::cout << "Random seed: " << random_seed << '\n';
    std::cout << "Hot-key workload: "
              << static_cast<int>(hot_key_probability * 100)
              << "% of searches target "
              << hot_key_count << " keys\n\n";

    print_results(results);

    if (!write_results_csv(results)) {
        return 1;
    }

    std::cout << "\nResults written to "
              << results_file.string() << '\n';

    return 0;
}