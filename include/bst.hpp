#include<utility>
#include"node.hpp"

template<typename T>
class BST  {
private:
    std::shared_ptr<Node<T>> root;

    void _insert(std::shared_ptr<Node<T>> cur, std::shared_ptr<Node<T>> newnode) {
        if(*newnode < *cur) {
            if(cur->left == nullptr) {
                cur->left = newnode;
                newnode->parent = cur;
            }
            else _insert(cur->left, newnode);
        } else if(*newnode > *cur) {
            if(cur->right == nullptr) {
                cur->right = newnode;
                newnode->parent = cur;
            }
            else _insert(cur->right, newnode);
        } else { return; }
    }

    int _find(T t, std::shared_ptr<Node<T>> cur, int depth) {
        if(cur==nullptr) return depth;
        else if(*cur==t) return depth;
        else if(*cur > t) return _find(t, cur->left, depth+1);
        else return _find(t, cur->right, depth+1);
    }

    void _print(std::shared_ptr<Node<T>> cur, std::string indent) {
        if(cur->left) _print(cur->left, indent+"-");
        std::cout << indent << cur->data << std::endl;
        if(cur->right) _print(cur->right, indent+"-");
        
    } 

public:
    BST()
        : root(nullptr)
    {}

    void insert(T t) {
        std::shared_ptr<Node<T>> node = std::make_shared<Node<T>>(t);
        if(root==nullptr) {
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