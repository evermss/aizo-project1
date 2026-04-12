#ifndef TEMPLATEARRAY_H
#define TEMPLATEARRAY_H

template <typename T>
class TemplateArray {
private:
    T* data;
    int size;
    int capacity;

    void resize(int newCapacity) {
        T* newData = new T[newCapacity];

        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    TemplateArray() {
        data = nullptr;
        size = 0;
        capacity = 0;
    }

    ~TemplateArray() {
        delete[] data;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }

    void pushBack(const T& value) {
        if (size == capacity) {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }

        data[size] = value;
        size++;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }
};

#endif