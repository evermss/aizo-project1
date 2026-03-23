#include "DynamicArray.h"

DynamicArray::DynamicArray() {
    data = nullptr;
    size = 0;
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

void DynamicArray::resize(int newSize) {
    delete[] data;
    size = newSize;

    if (size > 0) {
        data = new int[size];
    } else {
        data = nullptr;
    }
}

void DynamicArray::clear() {
    delete[] data;
    data = nullptr;
    size = 0;
}

int DynamicArray::getSize() const {
    return size;
}

int& DynamicArray::operator[](int index) {
    return data[index];
}

const int& DynamicArray::operator[](int index) const {
    return data[index];
}