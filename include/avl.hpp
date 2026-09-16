#pragma once

#include<memory>
#include<iostream>

template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
struct AvlNode {
    T element;
    sptr<AvlNode> left;
    sptr<AvlNode> right;
    int height;

    AvlNode(
        const T & t,
        sptr<AvlNode> left = nullptr,
        sptr<AvlNode> right = nullptr,
        int height = 0
    ) : element(t), left(left), right(right), height(height)
    {}
};

template<typename T>
class AvlTree {
    sptr<AvlNode<T>> root;

    void _insert(const T & t, sptr<AvlNode<T>> & node) {
        if(node == nullptr) 
            node = std::make_shared<AvlNode<T>>(t);
        else if(t < node->element) {
            _insert(t, node->left);
            if( height(node->left) - height(node->right) == 2) {
                if( t < node->left->element) {
                    singleWithLeftChild(node);
                }
                else {
                    doubleWithLeftChild(node);
                }
            }
        } else if(node->element < t) {
            _insert(t, node->right);
            if( height(node->right) - height(node->left) == 2) {
                if(node->right->element < t) {
                    singleWithRightChild(node);
                }
                else {
                    doubleWithRightChild(node);
                }
            }
        } else {
            ; // do nothing
        }
        node->height = std::max(
            height(node->left), 
            height(node->right)
        ) + 1;
    }

    void singleWithLeftChild(sptr<AvlNode<T>> & parent) {
        sptr<AvlNode<T>> left_child = parent->left;
        parent->left = left_child->right;
        left_child->right = parent;
        parent->height = std::max(
            height(parent->left),
            height(parent->right)
        ) + 1;
        left_child->height = std::max(
            height(left_child->left),
            height(left_child->right)
        ) + 1;
        parent = left_child;
    }

    void singleWithRightChild(sptr<AvlNode<T>> & parent) {
        sptr<AvlNode<T>> right_child = parent->right;
        parent->right = right_child->left;
        right_child->left = parent;
        parent->height = std::max(
            height(parent->left),
            height(parent->right)
        ) + 1;
        right_child->height = std::max(
            height(right_child->left),
            height(right_child->right)
        ) + 1;
        parent = right_child;
    }

    void doubleWithLeftChild(sptr<AvlNode<T>> & parent) {
        singleWithRightChild(parent->left);
        singleWithLeftChild(parent);
    }

    void doubleWithRightChild(sptr<AvlNode<T>> & parent) {
        singleWithLeftChild(parent->right);
        singleWithRightChild(parent);
    }

    int height(sptr<AvlNode<T>> node) {
        return node == nullptr ? -1 : node->height;
    }

    int _find(const T & t, sptr<AvlNode<T>> node) {
        if(node==nullptr) return 0;
        else if(t == node->element) return 0;
        else if( t < node->element) return 1 + _find(t, node->left);
        else if( t > node->element) return 1 + _find(t, node->right);
        else { return 0;
            }
    }

    void _print_tree(sptr<AvlNode<T>> & node, std::string indent) {
        if(!node) return;
        _print_tree(node->right, indent+"*");
        std::cout << indent 
                    << node->element 
                    << "(" << height(node) << ")"
                    << std::endl;
        _print_tree(node->left, indent+"*");
    }

public:
    AvlTree() : root(nullptr) {}

    void insert(const T & t) {
        _insert(t, root);
    }

    void print_tree() {
        _print_tree(root, "");
    }

    int find(const T & t) {
        return _find(t, root);
    }

};
