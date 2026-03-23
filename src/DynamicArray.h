#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

class DynamicArray {
private:
    int* data;
    int size;

public:
    DynamicArray();
    ~DynamicArray();

    void resize(int newSize);
    void clear();
    int getSize() const;

    int& operator[](int index);
    const int& operator[](int index) const;
};

#endif