#include <iostream>
#include <chrono>
#include "FileLoader.h"
#include "DynamicArray.h"
#include "QuickSort.h"
#include "SortChecker.h"

void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Program wczytuje dane z pliku, sortuje je QuickSortem\n";
    std::cout << "i sprawdza poprawnosc sortowania.\n\n";
    std::cout << "Format pliku wejsciowego:\n";
    std::cout << "1 linia - liczba elementow\n";
    std::cout << "kolejne linie - wartosci do sortowania\n\n";
    std::cout << "Aktualnie program korzysta z pliku: ../data.txt\n";
}

int main() {
    showHelp();
    std::cout << std::endl;

    DynamicArray arr;

    if (FileLoader::loadFromFile("../data.txt", arr)) {
        std::cout << "Dane przed sortowaniem:\n";
        for (int i = 0; i < arr.getSize(); i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        QuickSort::sort(arr, 0, arr.getSize() - 1);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Dane po sortowaniu:\n";
        for (int i = 0; i < arr.getSize(); i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        if (SortChecker::isSorted(arr)) {
            std::cout << "Tablica jest poprawnie posortowana.\n";
        } else {
            std::cout << "Blad sortowania.\n";
        }

        std::cout << "Czas sortowania: " << duration.count() << " mikrosekund\n";
    } else {
        std::cout << "Blad wczytywania pliku.\n";
    }

    return 0;
}