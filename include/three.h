#pragma once

#include <stdexcept>
#include <stack>
#include <string>
#include "polinom.h"

class AVLTree {
    struct TreeNode {
        std::string key;
        Polinom value;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        int height = 1;

        TreeNode(const std::string& k, const Polinom& v)
            : key(k), value(v) {
        }
    };

    TreeNode* root = nullptr;
    int elementCount = 0;

    int getHeight(TreeNode* node) const {
        return node ? node->height : 0;
    }

    int getBalanceFactor(TreeNode* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(TreeNode* node) {
        if (node) node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    TreeNode* rotateRight(TreeNode* y) {
        TreeNode* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    TreeNode* rotateLeft(TreeNode* x) {
        TreeNode* y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    TreeNode* rebalanceNode(TreeNode* node) {
        if (!node) return node;

        updateHeight(node);
        int balance = getBalanceFactor(node);

        if (balance > 1) {
            if (getBalanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1) {
            if (getBalanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    TreeNode* insertNode(TreeNode* node, const std::string& key, const Polinom& value) {
        if (!node) {
            elementCount++;
            return new TreeNode(key, value);
        }

        if (key < node->key)
            node->left = insertNode(node->left, key, value);
        else if (key > node->key)
            node->right = insertNode(node->right, key, value);
        else
            throw std::runtime_error("Duplicate key detected");

        return rebalanceNode(node);
    }

    TreeNode* findMinNode(TreeNode* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    TreeNode* removeNode(TreeNode* node, const std::string& key) {
        if (!node) return node;

        if (key < node->key)
            node->left = removeNode(node->left, key);
        else if (key > node->key)
            node->right = removeNode(node->right, key);
        else {
            if (!node->left || !node->right) {
                TreeNode* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    *node = *temp;
                }
                delete temp;
                elementCount--;
            }
            else {
                TreeNode* temp = findMinNode(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = removeNode(node->right, temp->key);
            }
        }
        return rebalanceNode(node);
    }

    bool compareTrees(TreeNode* a, TreeNode* b) const {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a->key == b->key && a->value == b->value &&
            compareTrees(a->left, b->left) && compareTrees(a->right, b->right);
    }

    TreeNode* cloneTree(TreeNode* node) const {
        if (!node) return nullptr;
        TreeNode* newNode = new TreeNode(node->key, node->value);
        newNode->left = cloneTree(node->left);
        newNode->right = cloneTree(node->right);
        newNode->height = node->height;
        return newNode;
    }

    void clearTree(TreeNode* node) {
        if (node) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

public:
    AVLTree() = default;
    AVLTree(const AVLTree& src) : root(cloneTree(src.root)), elementCount(src.elementCount) {}
    ~AVLTree() { clearTree(root); }

    AVLTree& operator=(const AVLTree& rhs) {
        if (this != &rhs) {
            clearTree(root);
            root = cloneTree(rhs.root);
            elementCount = rhs.elementCount;
        }
        return *this;
    }

    void insert(const std::string& key, const Polinom& value) {
        root = insertNode(root, key, value);
    }

    void erase(const std::string& key) {
        root = removeNode(root, key);
    }

    Polinom find(const std::string& key) const {
        TreeNode* current = root;
        while (current) {
            if (key < current->key)
                current = current->left;
            else if (key > current->key)
                current = current->right;
            else
                return current->value;
        }
        throw std::runtime_error("Key not found in tree");
    }

    int size() const { return elementCount; }
    int height() const { return getHeight(root); }
    bool empty() const { return !root; }

    bool operator==(const AVLTree& other) const {
        return compareTrees(root, other.root);
    }

    bool operator!=(const AVLTree& other) const {
        return !(*this == other);
    }

    class Iterator {
    private:
        std::stack<TreeNode*> nodes;
        TreeNode* current;

        void pushLeft(TreeNode* node) {
            while (node != nullptr) {
                nodes.push(node);
                node = node->left;
            }
        }

    public:
        Iterator(TreeNode* root) : current(nullptr) {
            pushLeft(root);
            if (!nodes.empty()) {
                current = nodes.top();
                nodes.pop();
            }
        }

        Iterator() : current(nullptr) {}

        const std::string& key() const {
            if (current == nullptr) {
                throw std::out_of_range("Iterator is out of range");
            }
            return current->key;
        }

        const Polinom& value() const {
            if (current == nullptr) {
                throw std::out_of_range("Iterator is out of range");
            }
            return current->value;
        }

        Iterator& operator++() {
            if (current == nullptr) {
                return *this;
            }

            pushLeft(current->right);
            if (nodes.empty()) {
                current = nullptr;
            }
            else {
                current = nodes.top();
                nodes.pop();
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() const { return Iterator(root); }
    Iterator end() const { return Iterator(); }
};