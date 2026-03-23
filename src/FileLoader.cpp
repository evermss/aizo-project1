#include "FileLoader.h"
#include <fstream>
#include <iostream>

bool FileLoader::loadFromFile(const std::string& fileName, DynamicArray& arr) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Blad otwarcia pliku: " << fileName << "\n";
        return false;
    }

    int n;
    file >> n;

    if (file.fail() || n < 0) {
        std::cout << "Niepoprawny format pliku.\n";
        return false;
    }

    arr.clear();
    arr.resize(n);

    for (int i = 0; i < n; i++) {
        file >> arr[i];

        if (file.fail()) {
            std::cout << "Blad podczas wczytywania danych.\n";
            return false;
        }
    }

    return true;
}

bool FileLoader::saveToFile(const std::string& fileName, const DynamicArray& arr) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Blad zapisu do pliku: " << fileName << "\n";
        return false;
    }

    file << arr.getSize() << "\n";
    for (int i = 0; i < arr.getSize(); i++) {
        file << arr[i] << "\n";
    }

    return true;
}