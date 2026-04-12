#ifndef STACK_H
#define STACK_H

#include "structures/LinearStructure.h"

class Stack : public LinearStructure {
private:
    struct Node {
        int value;
        Node* next;
    };

    Node* topNode;
    int size;

    Node* getNode(int index) const;

public:
    Stack();
    ~Stack() override;

    int getSize() const override;
    void clear() override;
    void pushBack(int value) override;
    int get(int index) const override;
    void set(int index, int value) override;
};

#endif