#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "structures/LinearStructure.h"

class DynamicArray : public LinearStructure {
private:
    int* data;
    int size;
    int capacity;

    void reserve(int newCapacity);

public:
    DynamicArray();
    ~DynamicArray() override;

    int getSize() const override;
    void clear() override;
    void pushBack(int value) override;
    int get(int index) const override;
    void set(int index, int value) override;

    void resize(int newSize);

    int& operator[](int index);
    const int& operator[](int index) const;
};

#endif