#include "FileLoader.h"
#include <fstream>
#include <iostream>

bool FileLoader::loadFromFile(const std::string& fileName, DynamicArray& arr) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Blad otwarcia pliku\n";
        return false;
    }

    int n;
    file >> n;

    arr.resize(n);

    for (int i = 0; i < n; i++) {
        file >> arr[i];
    }

    file.close();
    return true;
}