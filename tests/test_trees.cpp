#include "avl.hpp"
#include "bst.hpp"
#include "splay.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace {

int passed_tests = 0;
int failed_tests = 0;

void expect_true(const bool condition, const std::string& test_name) {
    if (condition) {
        std::cout << "[PASS] " << test_name << '\n';
        ++passed_tests;
        return;
    }

    std::cerr << "[FAIL] " << test_name << '\n';
    ++failed_tests;
}

void test_bst_finds_inserted_values() {
    BST<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    expect_true(tree.find(10) == 0, "BST finds its root");
    expect_true(tree.find(5) == 1, "BST finds its left child");
    expect_true(tree.find(15) == 1, "BST finds its right child");
}

void test_avl_balances_ordered_insertions() {
    AvlTree<int> tree;

    for (int value = 0; value < 100; ++value) {
        tree.insert(value);
    }

    const int depth = tree.find(99);

    expect_true(
        depth < 10,
        "AVL keeps ordered insertions logarithmic"
    );
}

void test_splay_moves_found_value_to_root() {
    Splay<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);

    tree.find(10);

    expect_true(
        tree.find(10) == 0,
        "Splay moves a found value to the root"
    );
}

void test_splay_moves_inserted_value_to_root() {
    Splay<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    expect_true(
        tree.find(15) == 0,
        "Splay moves a newly inserted value to the root"
    );
}

void test_splay_preserves_values_after_rotations() {
    Splay<int> tree;

    for (int value = 0; value < 50; ++value) {
        tree.insert(value);
    }

    bool all_values_found = true;

    for (int value = 0; value < 50; ++value) {
        const int first_depth = tree.find(value);
        const int second_depth = tree.find(value);

        if (first_depth < 0 || second_depth != 0) {
            all_values_found = false;
            break;
        }
    }

    expect_true(
        all_values_found,
        "Splay preserves all values after repeated rotations"
    );
}

void test_duplicate_insertions_do_not_break_trees() {
    BST<int> bst;
    AvlTree<int> avl;
    Splay<int> splay;

    for (int i = 0; i < 3; ++i) {
        bst.insert(42);
        avl.insert(42);
        splay.insert(42);
    }

    expect_true(bst.find(42) == 0, "BST handles duplicate insertions");
    expect_true(avl.find(42) == 0, "AVL handles duplicate insertions");
    expect_true(splay.find(42) == 0, "Splay handles duplicate insertions");
}

}  // namespace

int main() {
    test_bst_finds_inserted_values();
    test_avl_balances_ordered_insertions();
    test_splay_moves_found_value_to_root();
    test_splay_moves_inserted_value_to_root();
    test_splay_preserves_values_after_rotations();
    test_duplicate_insertions_do_not_break_trees();

    std::cout << "\nPassed: " << passed_tests
              << ", Failed: " << failed_tests << '\n';

    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}