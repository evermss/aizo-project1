#include "FileLoader.h"
#include <fstream>
#include <iostream>

// Wczytuje dane z pliku do struktury liniowej.
// Pierwsza linia zawiera liczbe elementow, kolejne linie wartosci.
bool FileLoader::loadFromFile(const std::string& fileName, LinearStructure& structure) {
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

    structure.clear();

    for (int i = 0; i < n; i++) {
        int value;
        file >> value;

        if (file.fail()) {
            std::cout << "Blad podczas wczytywania danych.\n";
            return false;
        }

        structure.pushBack(value);
    }

    return true;
}

// Zapisuje strukture do pliku w tym samym formacie:
// najpierw liczba elementow, potem kolejne wartosci.
bool FileLoader::saveToFile(const std::string& fileName, const LinearStructure& structure) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Blad zapisu do pliku: " << fileName << "\n";
        return false;
    }

    file << structure.getSize() << "\n";

    for (int i = 0; i < structure.getSize(); i++) {
        file << structure.get(i) << "\n";
    }

    return true;
}