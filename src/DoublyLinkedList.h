#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "LinearStructure.h"

class DoublyLinkedList : public LinearStructure {
private:
    struct Node {
        int value;
        Node* prev;
        Node* next;
    };

    Node* head;
    Node* tail;
    int size;

    // Zwraca wskaznik na node o danym indeksie
    Node* getNode(int index) const;

public:
    DoublyLinkedList();
    ~DoublyLinkedList() override;

    int getSize() const override;
    void clear() override;
    void pushBack(int value) override;
    int get(int index) const override;
    void set(int index, int value) override;
};

#endif