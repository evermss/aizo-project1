#include "structures/DoublyLinkedList.h"

DoublyLinkedList::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

// Szuka node po indeksie
DoublyLinkedList::Node* DoublyLinkedList::getNode(int index) const {
    Node* current;

    if (index < size / 2) {
        current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = tail;
        for (int i = size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    return current;
}

int DoublyLinkedList::getSize() const {
    return size;
}

// Usuwa cala liste
void DoublyLinkedList::clear() {
    Node* current = head;

    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Dodaje element na koniec
void DoublyLinkedList::pushBack(int value) {
    Node* newNode = new Node{value, nullptr, nullptr};

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

int DoublyLinkedList::get(int index) const {
    return getNode(index)->value;
}

void DoublyLinkedList::set(int index, int value) {
    getNode(index)->value = value;
}