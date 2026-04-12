#include "BinarySearchTree.h"

// Konstruktor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    size = 0;
}

// Destruktor
BinarySearchTree::~BinarySearchTree() {
    clear();
}

// Dodaje element do drzewa
void BinarySearchTree::insertNode(Node*& node, int value) {
    if (node == nullptr) {
        node = new Node{value, nullptr, nullptr};
        return;
    }

    if (value < node->value) {
        insertNode(node->left, value);
    } else {
        insertNode(node->right, value);
    }
}

// Usuwa cale drzewo
void BinarySearchTree::clearNode(Node* node) const {
    if (node == nullptr) {
        return;
    }

    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

// Przepisuje dane rosnaco do tablicy
void BinarySearchTree::inOrderFill(Node* node, int* values, int& index) const {
    if (node == nullptr) {
        return;
    }

    inOrderFill(node->left, values, index);
    values[index] = node->value;
    index++;
    inOrderFill(node->right, values, index);
}

// Zwraca rozmiar
int BinarySearchTree::getSize() const {
    return size;
}

// Czyści drzewo
void BinarySearchTree::clear() {
    clearNode(root);
    root = nullptr;
    size = 0;
}

// Dodaje element
void BinarySearchTree::pushBack(int value) {
    insertNode(root, value);
    size++;
}

// Zwraca element po indeksie po przejsciu inorder
int BinarySearchTree::get(int index) const {
    int* values = new int[size];
    int currentIndex = 0;

    inOrderFill(root, values, currentIndex);

    int result = values[index];
    delete[] values;

    return result;
}

// Ustawia wartosc pod indeksem
void BinarySearchTree::set(int index, int value) {
    int* values = new int[size];
    int currentIndex = 0;

    inOrderFill(root, values, currentIndex);
    values[index] = value;

    clear();

    for (int i = 0; i < currentIndex; i++) {
        pushBack(values[i]);
    }

    delete[] values;
}