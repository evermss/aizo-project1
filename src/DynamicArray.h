#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

class DynamicArray {
private:
    int* data;
    int size;

public:
    DynamicArray();
    ~DynamicArray();

    void resize(int newSize);   // Zmiana rozmiaru tablicy
    void clear();               // Usuniecie wszystkich danych
    int getSize() const;        // Zwraca rozmiar tablicy

    int& operator[](int index);             // Dostep do elementu
    const int& operator[](int index) const; // Dostep dla const
};

#endif