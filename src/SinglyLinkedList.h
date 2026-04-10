#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include "LinearStructure.h"

class SinglyLinkedList : public LinearStructure {
private:
    struct Node {
        int value;
        Node* next;
    };

    Node* head;
    Node* tail;
    int size;

    Node* getNode(int index) const;

public:
    SinglyLinkedList();
    ~SinglyLinkedList() override;

    int getSize() const override;
    void clear() override;
    void pushBack(int value) override;
    int get(int index) const override;
    void set(int index, int value) override;
};

#endif