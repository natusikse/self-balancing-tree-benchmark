#pragma once

#include<memory>
#include<optional>

/***
    Node class
*/
template<typename T>
class Node {
public:
    Node() {}
    Node(T t) {
        data = t;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
    bool operator==(const T & t) {
        return data == t;
    }
    bool operator<(const T & t) {
        return data < t;
    }
    bool operator>(const T & t) {
        return data > t;
    }
    bool operator==(const Node & n) {
        return data == n.data;
    }
    bool operator<(const Node & n) {
        return data < n.data;
    }
    bool operator>(const Node & n) {
        return data > n.data;
    }
    T data;
    std::shared_ptr<Node<T>> left;
    std::shared_ptr<Node<T>> right;
    std::shared_ptr<Node<T>> parent;
};