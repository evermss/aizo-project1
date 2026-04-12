#include "Stack.h"

// Konstruktor
Stack::Stack() {
    topNode = nullptr;
    size = 0;
}

// Destruktor
Stack::~Stack() {
    clear();
}

// Zwraca wezel o danym indeksie
Stack::Node* Stack::getNode(int index) const {
    Node* current = topNode;

    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

// Zwraca rozmiar
int Stack::getSize() const {
    return size;
}

// Czysci stos
void Stack::clear() {
    Node* current = topNode;

    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    topNode = nullptr;
    size = 0;
}

// Dodaje element na stos
void Stack::pushBack(int value) {
    Node* newNode = new Node{value, topNode};
    topNode = newNode;
    size++;
}

// Zwraca element po indeksie
int Stack::get(int index) const {
    return getNode(index)->value;
}

// Ustawia element po indeksie
void Stack::set(int index, int value) {
    getNode(index)->value = value;
}