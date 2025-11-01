//
// Created by Steph on 10/31/2025.
//

#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

template <typename T>
class AVLNode {
public:
    T key;
    AVLNode * left;
    AVLNode * right;
    int height;

    AVLNode(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int height(AVLNode<T>* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int balanceFactor(AVLNode<T>* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode<T>* rightRotation(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = std::max(height(y->left), height(y->right)) + 1;

        x->height = std::max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode<T>* leftRotation(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode<T>* insert(AVLNode<T>* node, T key) {
        if (node == nullptr)
            return new AVLNode<T>(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = balanceFactor(node);

        if (balance > 1 && key < node->left->key)
            return rightRotation(node);

        if (balance < -1 && key > node->right->key)
            return leftRotation(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotation(node->left);
            return rightRotation(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = leftRotation(node->right);
            return leftRotation(node);
        }

        return node;
    }

    AVLNode<T>* minValueNode(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }

        return current;
    }

    AVLNode<T>* deleteNode(AVLNode<T>* root, T k) {
        if (root == nullptr)
            return root;

        if (k < root->key)
            root->left = deleteNode(root->left, k);
        else if (k > root->key)
            root->right = deleteNode(root->right, k);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode<T>* temp = root->left ?root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                    delete temp;
                }
            } else {
                AVLNode<T>* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (root == nullptr)
            return root;

        root->height = 1 + std::max(height(root->left), height(root->right));

        int balance = balanceFactor(root);

        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotation(root);

        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotation(root->left);
            return rightRotation(root);
        }

        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotation(root);

        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rightRotation(root->right);
            return leftRotation(root);
        }

        return root;
    }

    void InOrder(AVLNode<T>* root) {
        if (root != nullptr) {
            InOrder(root->left);
            std::cout << root->key << " ";
            InOrder(root->right);
        }
    }

    void serialize(AVLNode<T>* root, std::vector<int>& v) {
        if (root == nullptr) {
            v.push_back(-1);
        } else {
            v.push_back(root->key);
            serialize(root->left, v);
            serialize(root->right, v);
        }

        return;
    }

    AVLNode<T>* deserialize(int &i, std::vector<int>& v) {
        if (v[i] == -1) {
            i++;
            return nullptr;
        }

        AVLNode<T>* root = new AVLNode<T>(v[i]);
        i++;

        root->left = deserialize(i, v);
        root->right = deserialize(i, v);

        return root;
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(T k) { root = insert(root, k); }
    void remove(const T k) { root = deleteNode(root, k); }

    void printInOrder() {
        InOrder(root);
        std::cout << std::endl;
    }

    std::string serialize() {
        std::vector<int> tokens;
        serialize(root, tokens);

        std::string result;

        for (int i = 0; i < tokens.size(); i++) {

            if (tokens[i] == -1) {
                result += "x";
            } else {
                result += std::to_string(tokens[i]);
            }

            if (i != tokens.size() - 1) {
                result += ", ";
            }
        }

        return result;
    }

    AVLTree<T> deserialize(std::string& str) {
        int i = 0;
        std::vector<int> v;
        std::stringstream ss(str);
        std::string line;

        while (std::getline(ss, line, ',')) {
            if (line == " x") {
                v.push_back(-1);
            }
            else {
                v.push_back(std::stoi(line));
            }
        }

        AVLTree<T> tree;
        tree.root = deserialize(i, v);
        return tree;
    }
};



#endif //AVLTREE_H
