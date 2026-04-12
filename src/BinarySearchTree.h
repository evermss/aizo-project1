#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "LinearStructure.h"

class BinarySearchTree : public LinearStructure {
private:
    struct Node {
        int value;
        Node* left;
        Node* right;
    };

    Node* root;
    int size;

    void insertNode(Node*& node, int value);
    void clearNode(Node* node) const;
    void inOrderFill(Node* node, int* values, int& index) const;

public:
    BinarySearchTree();
    ~BinarySearchTree() override;

    int getSize() const override;
    void clear() override;
    void pushBack(int value) override;
    int get(int index) const override;
    void set(int index, int value) override;
};

#endif