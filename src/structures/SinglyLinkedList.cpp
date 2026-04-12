#include "structures/SinglyLinkedList.h"

// Konstruktor listy
SinglyLinkedList::SinglyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Destruktor listy
SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

// Zwraca wskaznik na element o danym indeksie
SinglyLinkedList::Node* SinglyLinkedList::getNode(int index) const {
    Node* current = head;

    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

// Zwraca rozmiar listy
int SinglyLinkedList::getSize() const {
    return size;
}

// Usuwa cala liste
void SinglyLinkedList::clear() {
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

// Dodaje element na koniec listy
void SinglyLinkedList::pushBack(int value) {
    Node* newNode = new Node{value, nullptr};

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

// Zwraca wartosc pod indeksem
int SinglyLinkedList::get(int index) const {
    return getNode(index)->value;
}

// Ustawia wartosc pod indeksem
void SinglyLinkedList::set(int index, int value) {
    getNode(index)->value = value;
}