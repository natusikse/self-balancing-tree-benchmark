#pragma once

#include "node.hpp"

#include <iostream>
#include <memory>
#include <string>

template <typename T> class Splay {
private:
  using NodePointer = std::shared_ptr<Node<T>>;

  NodePointer root;

  void rotate_left(const NodePointer &pivot) {
    const NodePointer child = pivot->right;

    if (!child) {
      return;
    }

    pivot->right = child->left;

    if (child->left) {
      child->left->parent = pivot;
    }

    child->parent = pivot->parent;

    if (!pivot->parent) {
      root = child;
    } else if (pivot == pivot->parent->left) {
      pivot->parent->left = child;
    } else {
      pivot->parent->right = child;
    }

    child->left = pivot;
    pivot->parent = child;
  }

  void rotate_right(const NodePointer &pivot) {
    const NodePointer child = pivot->left;

    if (!child) {
      return;
    }

    pivot->left = child->right;

    if (child->right) {
      child->right->parent = pivot;
    }

    child->parent = pivot->parent;

    if (!pivot->parent) {
      root = child;
    } else if (pivot == pivot->parent->left) {
      pivot->parent->left = child;
    } else {
      pivot->parent->right = child;
    }

    child->right = pivot;
    pivot->parent = child;
  }

  void splay(const NodePointer &node) {
    if (!node) {
      return;
    }

    while (node->parent) {
      const NodePointer parent = node->parent;
      const NodePointer grandparent = parent->parent;

      if (!grandparent) {
        if (node == parent->left) {
          rotate_right(parent);
        } else {
          rotate_left(parent);
        }

        continue;
      }

      if (node == parent->left && parent == grandparent->left) {
        rotate_right(grandparent);
        rotate_right(parent);
      } else if (node == parent->right && parent == grandparent->right) {
        rotate_left(grandparent);
        rotate_left(parent);
      } else if (node == parent->right && parent == grandparent->left) {
        rotate_left(parent);
        rotate_right(grandparent);
      } else {
        rotate_right(parent);
        rotate_left(grandparent);
      }
    }

    root = node;
  }

  void print_subtree(const NodePointer &node,
                     const std::string &indentation) const {
    if (!node) {
      return;
    }

    print_subtree(node->right, indentation + "*");
    std::cout << indentation << node->data << '\n';
    print_subtree(node->left, indentation + "*");
  }

public:
  Splay() : root(nullptr) {}

  void insert(const T &value) {
    const NodePointer new_node = std::make_shared<Node<T>>(value);

    if (!root) {
      root = new_node;
      return;
    }

    NodePointer current = root;

    while (current) {
      if (value < current->data) {
        if (current->left) {
          current = current->left;
          continue;
        }

        current->left = new_node;
        new_node->parent = current;
        splay(new_node);
        return;
      }

      if (value > current->data) {
        if (current->right) {
          current = current->right;
          continue;
        }

        current->right = new_node;
        new_node->parent = current;
        splay(new_node);
        return;
      }

      splay(current);
      return;
    }
  }

  int find(const T &value) {
    NodePointer current = root;
    NodePointer last_accessed = nullptr;
    int depth = 0;

    while (current) {
      last_accessed = current;

      if (value == current->data) {
        splay(current);
        return depth;
      }

      if (value < current->data) {
        current = current->left;
      } else {
        current = current->right;
      }

      ++depth;
    }

    if (last_accessed) {
      splay(last_accessed);
    }

    return depth;
  }

  void print() const { print_subtree(root, ""); }
};