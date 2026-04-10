#include <iostream>
#include <chrono>
#include <limits>
#include <string>
#include "FileLoader.h"
#include "DynamicArray.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "QuickSort.h"
#include "SortChecker.h"
#include "LinearStructure.h"

void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Dostepne tryby:\n";
    std::cout << "  help\n";
    std::cout << "  single <plik_wejsciowy> <struktura> [plik_wyjsciowy]\n";
    std::cout << "  research <plik_wejsciowy> <powtorzenia> <struktura>\n\n";

    std::cout << "Dostepne struktury:\n";
    std::cout << "  array  - tablica dynamiczna\n";
    std::cout << "  slist  - lista jednokierunkowa\n";
    std::cout << "  dlist  - lista dwukierunkowa\n\n";

    std::cout << "Przyklady:\n";
    std::cout << "  ./aizo-project1 help\n";
    std::cout << "  ./aizo-project1 single ../data.txt array\n";
    std::cout << "  ./aizo-project1 single ../data.txt slist ../sorted.txt\n";
    std::cout << "  ./aizo-project1 research ../data.txt 10 dlist\n\n";

    std::cout << "Format pliku wejsciowego:\n";
    std::cout << "  1 linia - liczba elementow\n";
    std::cout << "  kolejne linie - wartosci\n";
}

void printStructure(const LinearStructure& structure) {
    for (int i = 0; i < structure.getSize(); i++) {
        std::cout << structure.get(i) << " ";
    }
    std::cout << "\n";
}

bool parsePositiveInt(const char* text, int& value) {
    if (text == nullptr || text[0] == '\0') {
        return false;
    }

    int result = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }

        result = result * 10 + (text[i] - '0');
    }

    if (result <= 0) {
        return false;
    }

    value = result;
    return true;
}

bool isValidStructureName(const std::string& structureName) {
    return structureName == "array" ||
           structureName == "slist" ||
           structureName == "dlist";
}

template <typename Structure>
bool runSingleForStructure(const std::string& inputFile,
                           const std::string& outputFile,
                           bool saveOutput) {
    Structure structure;

    if (!FileLoader::loadFromFile(inputFile, structure)) {
        std::cout << "Blad wczytywania pliku.\n";
        return false;
    }

    std::cout << "Dane przed sortowaniem:\n";
    printStructure(structure);

    auto start = std::chrono::high_resolution_clock::now();

    if (structure.getSize() > 0) {
        QuickSort::sort(structure, 0, structure.getSize() - 1);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Dane po sortowaniu:\n";
    printStructure(structure);

    if (SortChecker::isSorted(structure)) {
        std::cout << "Struktura jest poprawnie posortowana.\n";
    } else {
        std::cout << "Blad sortowania.\n";
        return false;
    }

    std::cout << "Czas sortowania: " << duration.count() << " mikrosekund\n";

    if (saveOutput) {
        if (FileLoader::saveToFile(outputFile, structure)) {
            std::cout << "Zapisano wynik do pliku: " << outputFile << "\n";
        } else {
            std::cout << "Nie udalo sie zapisac wyniku.\n";
            return false;
        }
    }

    return true;
}

template <typename Structure>
bool runResearchForStructure(const std::string& inputFile, int repetitions) {
    if (repetitions <= 0) {
        std::cout << "Liczba powtorzen musi byc wieksza od 0.\n";
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        Structure structure;

        if (!FileLoader::loadFromFile(inputFile, structure)) {
            std::cout << "Blad wczytywania pliku.\n";
            return false;
        }

        auto start = std::chrono::high_resolution_clock::now();

        if (structure.getSize() > 0) {
            QuickSort::sort(structure, 0, structure.getSize() - 1);
        }

        auto end = std::chrono::high_resolution_clock::now();

        long long duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (!SortChecker::isSorted(structure)) {
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
    std::cout << "Powtorzenia: " << repetitions << "\n";
    std::cout << "Algorytm: QuickSort\n";
    std::cout << "Sredni czas: " << average << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    return true;
}

bool runSingleMode(const std::string& inputFile,
                   const std::string& structureName,
                   const std::string& outputFile,
                   bool saveOutput) {
    if (structureName == "array") {
        return runSingleForStructure<DynamicArray>(inputFile, outputFile, saveOutput);
    }

    if (structureName == "slist") {
        return runSingleForStructure<SinglyLinkedList>(inputFile, outputFile, saveOutput);
    }

    if (structureName == "dlist") {
        return runSingleForStructure<DoublyLinkedList>(inputFile, outputFile, saveOutput);
    }

    std::cout << "Nieznana struktura danych.\n";
    return false;
}

bool runResearchMode(const std::string& inputFile,
                     int repetitions,
                     const std::string& structureName) {
    if (structureName == "array") {
        return runResearchForStructure<DynamicArray>(inputFile, repetitions);
    }

    if (structureName == "slist") {
        return runResearchForStructure<SinglyLinkedList>(inputFile, repetitions);
    }

    if (structureName == "dlist") {
        return runResearchForStructure<DoublyLinkedList>(inputFile, repetitions);
    }

    std::cout << "Nieznana struktura danych.\n";
    return false;
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
        if (argc < 4) {
            std::cout << "Brak wymaganych argumentow do trybu single.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];
        std::string structureName = argv[3];

        if (!isValidStructureName(structureName)) {
            std::cout << "Niepoprawna struktura danych.\n";
            showHelp();
            return 1;
        }

        if (argc >= 5) {
            std::string outputFile = argv[4];
            return runSingleMode(inputFile, structureName, outputFile, true) ? 0 : 1;
        }

        return runSingleMode(inputFile, structureName, "", false) ? 0 : 1;
    }

    if (mode == "research") {
        if (argc < 5) {
            std::cout << "Brak wymaganych argumentow do trybu research.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];
        int repetitions = 0;
        std::string structureName = argv[4];

        if (!parsePositiveInt(argv[3], repetitions)) {
            std::cout << "Niepoprawna liczba powtorzen.\n";
            return 1;
        }

        if (!isValidStructureName(structureName)) {
            std::cout << "Niepoprawna struktura danych.\n";
            showHelp();
            return 1;
        }

        return runResearchMode(inputFile, repetitions, structureName) ? 0 : 1;
    }

    std::cout << "Nieznany tryb: " << mode << "\n";
    showHelp();
    return 1;
}