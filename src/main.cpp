#include <iostream>
#include <chrono>
#include <limits>
#include "FileLoader.h"
#include "DynamicArray.h"
#include "QuickSort.h"
#include "SortChecker.h"

void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Dostepne tryby:\n";
    std::cout << "  help\n";
    std::cout << "  single <plik_wejsciowy> [plik_wyjsciowy]\n";
    std::cout << "  research <plik_wejsciowy> <powtorzenia>\n\n";

    std::cout << "Przyklady:\n";
    std::cout << "  ./aizo-project1 help\n";
    std::cout << "  ./aizo-project1 single ../data.txt\n";
    std::cout << "  ./aizo-project1 single ../data.txt ../sorted.txt\n";
    std::cout << "  ./aizo-project1 research ../data.txt 10\n\n";

    std::cout << "Format pliku wejsciowego:\n";
    std::cout << "  1 linia - liczba elementow\n";
    std::cout << "  kolejne linie - wartosci\n";
}

void printArray(const DynamicArray& arr) {
    for (int i = 0; i < arr.getSize(); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

bool runSingleMode(const std::string& inputFile, const std::string& outputFile, bool saveOutput) {
    DynamicArray arr;

    if (!FileLoader::loadFromFile(inputFile, arr)) {
        std::cout << "Blad wczytywania pliku.\n";
        return false;
    }

    std::cout << "Dane przed sortowaniem:\n";
    printArray(arr);

    auto start = std::chrono::high_resolution_clock::now();
    QuickSort::sort(arr, 0, arr.getSize() - 1);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Dane po sortowaniu:\n";
    printArray(arr);

    if (SortChecker::isSorted(arr)) {
        std::cout << "Tablica jest poprawnie posortowana.\n";
    } else {
        std::cout << "Blad sortowania.\n";
        return false;
    }

    std::cout << "Czas sortowania: " << duration.count() << " mikrosekund\n";

    if (saveOutput) {
        if (FileLoader::saveToFile(outputFile, arr)) {
            std::cout << "Zapisano wynik do pliku: " << outputFile << "\n";
        } else {
            std::cout << "Nie udalo sie zapisac wyniku.\n";
            return false;
        }
    }

    return true;
}

bool runResearchMode(const std::string& inputFile, int repetitions) {
    if (repetitions <= 0) {
        std::cout << "Liczba powtorzen musi byc wieksza od 0.\n";
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        DynamicArray arr;

        if (!FileLoader::loadFromFile(inputFile, arr)) {
            std::cout << "Blad wczytywania pliku.\n";
            return false;
        }

        auto start = std::chrono::high_resolution_clock::now();
        QuickSort::sort(arr, 0, arr.getSize() - 1);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (!SortChecker::isSorted(arr)) {
            std::cout << "Blad sortowania w powtorzeniu: " << i + 1 << "\n";
            return false;
        }

        sum += duration;

        if (duration < minTime) {
            minTime = duration;
        }

        if (duration > maxTime) {
            maxTime = duration;
        }
    }

    double average = static_cast<double>(sum) / repetitions;

    std::cout << "=== TRYB BADAN ===\n";
    std::cout << "Plik: " << inputFile << "\n";
    std::cout << "Powtorzenia: " << repetitions << "\n";
    std::cout << "Algorytm: QuickSort\n";
    std::cout << "Sredni czas: " << average << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showHelp();
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "help") {
        showHelp();
        return 0;
    }

    if (mode == "single") {
        if (argc < 3) {
            std::cout << "Brak pliku wejsciowego.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];

        if (argc >= 4) {
            std::string outputFile = argv[3];
            return runSingleMode(inputFile, outputFile, true) ? 0 : 1;
        }

        return runSingleMode(inputFile, "", false) ? 0 : 1;
    }

    if (mode == "research") {
        if (argc < 4) {
            std::cout << "Brak wymaganych argumentow do trybu research.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];
        int repetitions = std::stoi(argv[3]);

        return runResearchMode(inputFile, repetitions) ? 0 : 1;
    }

    std::cout << "Nieznany tryb: " << mode << "\n";
    showHelp();
    return 1;
}