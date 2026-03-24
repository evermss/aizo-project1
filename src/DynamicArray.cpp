#include "DynamicArray.h"

DynamicArray::DynamicArray() {
    // Pusta tablica na start
    data = nullptr;
    size = 0;
}

DynamicArray::~DynamicArray() {
    // Zwolnienie zaalokowanej pamieci
    delete[] data;
}

void DynamicArray::resize(int newSize) {
    delete[] data;
    size = newSize;

    // Tworzenie nowej tablicy o zadanym rozmiarze
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
//zwraca liczbe elementow
int DynamicArray::getSize() const {
    return size;
}

int& DynamicArray::operator[](int index) {
    // Umozliwia zapis i odczyt przez arr[i]
    return data[index];
}

const int& DynamicArray::operator[](int index) const {
    return data[index];
}