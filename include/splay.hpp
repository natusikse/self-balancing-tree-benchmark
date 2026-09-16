/*
here is the main part of hw - i do not think there shiuld be any bugs but i faced a lot of them duting the implemenation of the splay method
the main two were - 'usage of undeclared functions' on 'insert' 'find' and 'print' so to fix it i have written helpers

and the other one was 'segmentation fault' which i suppose happened due to the lost of connection with the great-great-grandfather of the knot when turning zig-zag and zig-zag
so to fix it i added std::shared_ptr<Node<T>> gg = grandP->parent;  and used gg after - it helped to fix the bug, but i m not sure if it did not break the logic
and i also added the extra check - on the string 238

p. s. the benchmarks are in the file main.cpp - i did not know if in the second check we can add random numbers, so i made all of the numbers unique just in case
*/

#include <utility>
#include "node.hpp"
#include <iostream>
#include <string>
#include <iostream>

template<typename T>
class Splay {
private:
    std::shared_ptr<Node<T>> root;

    /* The splay method: remember the splay method is responsible for
        "splaying" the current node to the root of the tree.
        The node passed into the method is the node that was either:
            - just inserted
            - or found, using the find method.
        This is the node that must be "splayed" to the root of the tree.

        NOTE: Your implementation should be recursive.

        Please refer to the _insert and _find methods to see where this
        method is called. 
    */

// HELPER FUNCTIONS so find and insert work - i faced an error 'the usage of undeclared functions' - that's why i made them
// maybe it could have worked without those, but it works like that so i decided to leave them


void _insert(std::shared_ptr<Node<T>> cur, std::shared_ptr<Node<T>> newnode) {
    if (*newnode < *cur) {
        if (cur->left == nullptr) {
            cur->left = newnode;
            newnode->parent = cur;
            if (newnode->parent) _splay(newnode);  // then it's safe
        } else {
            _insert(cur->left, newnode);
        }
    } else if (*newnode > *cur) {
        if (cur->right == nullptr) {
            cur->right = newnode;
            newnode->parent = cur;
            if (newnode->parent) _splay(newnode);  // safe
        } else {
            _insert(cur->right, newnode);
        }
    } else {
        return;
    }
}

int _find(T t, std::shared_ptr<Node<T>> cur, int depth) {
    // if the current node is null, element not found — return current depth
    if (!cur) return depth;

    // if we found the element
    if (*cur == t) {
        _splay(cur); // bring it to the root
        return depth; // return the depth where it was found
    }

    // if element is smaller than current node's data — go left
    if (*cur > t) {
        if (cur->left) {
            return _find(t, cur->left, depth + 1);
        } else {
            _splay(cur); // splay the last accessed node
            return depth + 1; // element not found, return where search stopped
        }
    }

    // if element is larger - go right
    else {
        if (cur->right) {
            return _find(t, cur->right, depth + 1);
        } else {
            _splay(cur); // splay the last accessed node
            return depth + 1; // element not found
        }
    }
}


void _print(std::shared_ptr<Node<T>> cur, std::string indent) {
    if (!cur) return;
    if (cur->right) _print(cur->right, indent + "*");
    std::cout << indent << cur->data << std::endl;
    if (cur->left) _print(cur->left, indent + "*");
}
void _splay(std::shared_ptr<Node<T>> cur) {
    if (!cur) return;
    if (!cur->parent) { 
        root = cur;
        return;
    }

    while (cur->parent != nullptr) {
        std::shared_ptr<Node<T>> parent = cur->parent;
        if (!parent) break; // protection from nullptr
        std::shared_ptr<Node<T>> grandP = parent->parent;

        // SITUATION 1: zig - if parent has no grandparent
        if (grandP == nullptr) {
            if (cur == parent->left) {
                // right rotation around parent
                parent->left = cur->right;
                if (cur->right) cur->right->parent = parent;
                cur->right = parent;
                parent->parent = cur;
            } else {
                // left rotation around parent
                parent->right = cur->left;
                if (cur->left) cur->left->parent = parent;
                cur->left = parent;
                parent->parent = cur;
            }
            cur->parent = nullptr; // cur becomes root
            root = cur;
            break; // important! exit after zig
        }

        // important! save great-grandparent BEFORE rotations
        std::shared_ptr<Node<T>> gg = grandP->parent;

        // SITUATION 2: zig-zig — both on same side 
        if (cur == parent->left && parent == grandP->left) {
            // two right rotations
            grandP->left = parent->right;
            if (parent->right) parent->right->parent = grandP;
            parent->right = grandP;
            grandP->parent = parent;

            parent->left = cur->right;
            if (cur->right) cur->right->parent = parent;
            cur->right = parent;
            parent->parent = cur;

            // reconnect cur with great-grandparent if it exists
            cur->parent = gg;
            if (gg) {
                if (gg->left == grandP) gg->left = cur;
                else gg->right = cur;
            } else {
                root = cur;
            }

            continue; // going to the next iteration
        }

        else if (cur == parent->right && parent == grandP->right) {
            // two left rotations
            grandP->right = parent->left;
            if (parent->left) parent->left->parent = grandP;
            parent->left = grandP;
            grandP->parent = parent;

            parent->right = cur->left;
            if (cur->left) cur->left->parent = parent;
            cur->left = parent;
            parent->parent = cur;

            // reconnect cur with great-grandparent (if it exists)
            cur->parent = gg;
            if (gg) {
                if (gg->left == grandP) gg->left = cur;
                else gg->right = cur;
            } else {
                root = cur;
            }

            continue;
        }

        // SITUATION 3: zig-zag — different sides 
        if (cur == parent->right && parent == grandP->left) {
            // left around parent
            parent->right = cur->left;
            if (cur->left) cur->left->parent = parent;
            cur->left = parent;
            parent->parent = cur;

            // right around grandP
            grandP->left = cur->right;
            if (cur->right) cur->right->parent = grandP;
            cur->right = grandP;
            grandP->parent = cur;

            // reconnect cur with great-grandparent (if it exists)
            cur->parent = gg;
            if (gg) {
                if (gg->left == grandP) gg->left = cur;
                else gg->right = cur;
            } else {
                root = cur;
            }

            continue;
        }

        else if (cur == parent->left && parent == grandP->right) {
            // right around parent
            parent->left = cur->right;
            if (cur->right) cur->right->parent = parent;
            cur->right = parent;
            parent->parent = cur;

            // left around grandP
            grandP->right = cur->left;
            if (cur->left) cur->left->parent = grandP;
            cur->left = grandP;
            grandP->parent = cur;

            // reconnect cur with great-grandparent (if it exists)
            cur->parent = gg;
            if (gg) {
                if (gg->left == grandP) gg->left = cur;
                else gg->right = cur;
            } else {
                root = cur;
            }

            continue;
        }

        // protection - if none of the condictions didn't work
        break;
    }

    // making sure that root is correct
    if (cur && cur->parent == nullptr) {
        root = cur;
    }
}


public:
    Splay()
        : root(nullptr)
    {}

    void insert(T t) {
        std::shared_ptr<Node<T>> node = std::make_shared<Node<T>>(t);
        if (root == nullptr) {
            root = node;
        } else {
            _insert(root, node);
        }
    }

    int find(T t) {
        return _find(t, root, 0);
    }

    void print() {
        _print(root, "");
    }
};
