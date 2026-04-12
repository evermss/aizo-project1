#include "structures/DynamicArray.h"

DynamicArray::DynamicArray() {
    data = nullptr;
    size = 0;
    capacity = 0;
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

void DynamicArray::reserve(int newCapacity) {
    if (newCapacity <= capacity) {
        return;
    }

    int* newData = new int[newCapacity];

    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

int DynamicArray::getSize() const {
    return size;
}

void DynamicArray::clear() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}

void DynamicArray::pushBack(int value) {
    if (size == capacity) {
        int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        reserve(newCapacity);
    }

    data[size] = value;
    size++;
}

int DynamicArray::get(int index) const {
    return data[index];
}

void DynamicArray::set(int index, int value) {
    data[index] = value;
}

void DynamicArray::resize(int newSize) {
    if (newSize < 0) {
        return;
    }

    if (newSize > capacity) {
        reserve(newSize);
    }

    for (int i = size; i < newSize; i++) {
        data[i] = 0;
    }

    size = newSize;
}

int& DynamicArray::operator[](int index) {
    return data[index];
}

const int& DynamicArray::operator[](int index) const {
    return data[index];
}