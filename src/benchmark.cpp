#include "splay.hpp"
#include "avl.hpp"
#include "bst.hpp"
#include <iostream>
#include <cstdlib> // library for random thingies
using namespace std;

int main() {
    /*
    it was just a quick check of a splay tree
    Splay<int> tree;
    tree.insert(15);
    tree.insert(5);
    tree.insert(20);
    tree.print();
    */
    // the benchmark with numbers in trees in order
    BST<int> tree_bst;
    Splay<int> tree_splay;
    AvlTree<int> tree_avl;
    long long totalDep_inOrder_splay = 0;
    long long totalDep_inOrder_avl = 0;
    long long totalDep_inOrder_bst = 0;
    for (int i = 0; i <= 1000; i++){
        tree_bst.insert(i);
        tree_splay.insert(i);
        tree_avl.insert(i);
    }
    for (int i = 0; i < 100000; i++){
        int randomSearch = rand() % 1001; // the random number such that it doesn't get the big number
        totalDep_inOrder_splay += tree_splay.find(randomSearch); 
        totalDep_inOrder_avl += tree_avl.find(randomSearch); 
        totalDep_inOrder_bst += tree_bst.find(randomSearch);
    }
    cout << "the average depth of 100 000 random seraches in the bst tree in the right order of 1000 numbers is: " << totalDep_inOrder_bst/100000.0 << endl;
    cout << "the average depth of 100 000 random seraches in the splay tree in the right order of 1000 numbers is: " << totalDep_inOrder_splay/100000.0 << endl;
    cout << "the average depth of 100 000 random seraches in the avl tree in the right order of 1000 numbers is: " << totalDep_inOrder_avl/100000.0 <<endl;
    
    // the benchmark with numbers in trees in RANDOM order
    BST<int> tree_bst1;
    Splay<int> tree_splay1;
    AvlTree<int> tree_avl1;
    long long totalDep_inRandomOrder_splay = 0;
    long long totalDep_inRandomOrder_avl = 0;
    long long totalDep_inRandomOrder_bst = 0;
    // these are heeeded for avoiding repetitions
    const int MAX = 1001;
    int used[MAX];   // 0 = wasn't used, 1 = used
    for (int i = 0; i < MAX; i++) {
        used[i] = 0;
    }
    srand(time(0));  // initialization of random numbers
    for (int count = 0; count < 1000; count++) {
        int num = rand() % MAX;
        // while the number was used, taking a new one
        while (used[num] == 1) {
            num = rand() % MAX;
        }
        used[num] = 1; // note as 'used'
        // here is num going to be unique so we put it to the trees
        tree_bst1.insert(num);
        tree_splay1.insert(num);
        tree_avl1.insert(num);
        // tree_bst1.print(); -- supposed to be quick check but it crashed my laptop so do not uncomment
    }
    for (int i = 0; i < 100000; i++){
        int randomSearch = rand() % 1001; // the random number such that it doesn't get the big number
        totalDep_inRandomOrder_splay += tree_splay1.find(randomSearch); 
        totalDep_inRandomOrder_avl += tree_avl1.find(randomSearch); 
        totalDep_inRandomOrder_bst += tree_bst1.find(randomSearch);
    }

    cout << "the average depth of 100 000 random seraches in the bst tree in the random order of 1000 numbers is: " << totalDep_inRandomOrder_bst/100000.0 << endl;
    cout << "the average depth of 100 000 random seraches in the splay tree in the random order of 1000 numbers is: " << totalDep_inRandomOrder_splay/100000.0 << endl;
    cout << "the average depth of 100 000 random seraches in the avl tree in the right random of 1000 numbers is: " << totalDep_inRandomOrder_avl/100000.0 <<endl;
    
    
    return 0;
}

