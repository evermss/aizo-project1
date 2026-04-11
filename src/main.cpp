#include <iostream>
#include <chrono>
#include <limits>
#include <string>
#include <cstdlib>
#include <ctime>
#include "FileLoader.h"
#include "DynamicArray.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "QuickSorter.h"
#include "ShellSorter.h"
#include "BucketSorter.h"
#include "SortChecker.h"
#include "LinearStructure.h"
#include "CsvReportWriter.h"

// Pokazuje pomoc programu
void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Tryby:\n";
    std::cout << "  help\n";
    std::cout << "  single <plik> <struktura> <algorytm> [wariant] [plik_wyjsciowy]\n";
    std::cout << "  research <plik> <powtorzenia> <struktura> <algorytm> <plik_csv> [wariant]\n\n";

    std::cout << "Struktury:\n";
    std::cout << "  array\n";
    std::cout << "  slist\n";
    std::cout << "  dlist\n\n";

    std::cout << "Algorytmy:\n";
    std::cout << "  quick   - warianty: middle, random, extreme\n";
    std::cout << "  shell   - warianty: halving, knuth\n";
    std::cout << "  bucket\n\n";

    std::cout << "Przyklady:\n";
    std::cout << "  ./aizo-project1 single ../data.txt array quick\n";
    std::cout << "  ./aizo-project1 single ../data.txt slist quick random\n";
    std::cout << "  ./aizo-project1 single ../data.txt dlist shell knuth ../sorted.txt\n";
    std::cout << "  ./aizo-project1 research ../data.txt 10 array quick results.csv middle\n";
}

// Wypisuje zawartość struktury
void printStructure(const LinearStructure& structure) {
    for (int i = 0; i < structure.getSize(); i++) {
        std::cout << structure.get(i) << " ";
    }
    std::cout << "\n";
}

// Parsuje dodatnią liczbę całkowitą
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

// Sprawdza nazwę struktury
bool isValidStructureName(const std::string& structureName) {
    return structureName == "array" ||
           structureName == "slist" ||
           structureName == "dlist";
}

// Sprawdza nazwę algorytmu
bool isValidAlgorithmName(const std::string& algorithmName) {
    return algorithmName == "quick" ||
           algorithmName == "shell" ||
           algorithmName == "bucket";
}

// Sprawdza nazwę wariantu
bool isValidVariantName(const std::string& algorithmName, const std::string& variantName) {
    if (variantName.empty()) {
        return true;
    }

    if (algorithmName == "quick") {
        return variantName == "middle" ||
               variantName == "random" ||
               variantName == "extreme";
    }

    if (algorithmName == "shell") {
        return variantName == "halving" ||
               variantName == "knuth";
    }

    if (algorithmName == "bucket") {
        return false;
    }

    return false;
}

// Zamienia tekst na strategię pivota
QuickPivotStrategy getQuickPivotStrategy(const std::string& variantName) {
    if (variantName == "random") {
        return QuickPivotStrategy::Random;
    }

    if (variantName == "extreme") {
        return QuickPivotStrategy::Extreme;
    }

    return QuickPivotStrategy::Middle;
}

// Zamienia tekst na strategię odstępów
ShellGapStrategy getShellGapStrategy(const std::string& variantName) {
    if (variantName == "knuth") {
        return ShellGapStrategy::Knuth;
    }

    return ShellGapStrategy::Halving;
}

// Sortuje strukturę wybranym algorytmem
template <typename Structure>
bool sortStructure(Structure& structure,
                   const std::string& algorithmName,
                   const std::string& variantName) {
    if (algorithmName == "quick") {
        QuickPivotStrategy strategy = getQuickPivotStrategy(variantName);
        QuickSorter::sort(structure, 0, structure.getSize() - 1, strategy);
        return true;
    }

    if (algorithmName == "shell") {
        ShellGapStrategy strategy = getShellGapStrategy(variantName);
        ShellSorter::sort(structure, strategy);
        return true;
    }

    if (algorithmName == "bucket") {
        BucketSorter::sort(structure);
        return true;
    }

    return false;
}

// Tryb single dla wybranej struktury
template <typename Structure>
bool runSingleForStructure(const std::string& inputFile,
                           const std::string& algorithmName,
                           const std::string& variantName,
                           const std::string& outputFile,
                           bool saveOutput) {
    Structure structure;

    // Wczytuje dane
    if (!FileLoader::loadFromFile(inputFile, structure)) {
        std::cout << "Blad wczytywania pliku.\n";
        return false;
    }

    std::cout << "Dane przed sortowaniem:\n";
    printStructure(structure);

    // Mierzy tylko sortowanie
    auto startTime = std::chrono::high_resolution_clock::now();

    if (structure.getSize() > 0) {
        if (!sortStructure(structure, algorithmName, variantName)) {
            std::cout << "Niepoprawny algorytm.\n";
            return false;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    std::cout << "Dane po sortowaniu:\n";
    printStructure(structure);

    // Sprawdza poprawność
    if (!SortChecker::isSorted(structure)) {
        std::cout << "Blad sortowania.\n";
        return false;
    }

    std::cout << "Sortowanie poprawne.\n";
    std::cout << "Czas sortowania: " << duration.count() << " mikrosekund\n";

    // Zapisuje wynik do pliku
    if (saveOutput) {
        if (!FileLoader::saveToFile(outputFile, structure)) {
            std::cout << "Nie udalo sie zapisac pliku.\n";
            return false;
        }

        std::cout << "Zapisano wynik do pliku: " << outputFile << "\n";
    }

    return true;
}

// Tryb research dla wybranej struktury
template <typename Structure>
bool runResearchForStructure(const std::string& inputFile,
                             int repetitions,
                             const std::string& algorithmName,
                             const std::string& variantName,
                             const std::string& csvFile,
                             const std::string& structureName) {
    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        Structure structure;

        // Wczytuje dane od nowa w każdej iteracji
        if (!FileLoader::loadFromFile(inputFile, structure)) {
            std::cout << "Blad wczytywania pliku.\n";
            return false;
        }

        auto startTime = std::chrono::high_resolution_clock::now();

        if (structure.getSize() > 0) {
            if (!sortStructure(structure, algorithmName, variantName)) {
                std::cout << "Niepoprawny algorytm.\n";
                return false;
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();

        long long duration =
            std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        if (!SortChecker::isSorted(structure)) {
            std::cout << "Blad sortowania w powtorzeniu " << i + 1 << ".\n";
            return false;
        }

        std::cout << "Pomiar " << i + 1 << ": " << duration << " mikrosekund\n";

        sum += duration;

        if (duration < minTime) {
            minTime = duration;
        }

        if (duration > maxTime) {
            maxTime = duration;
        }
    }

    double averageTime = static_cast<double>(sum) / repetitions;

    std::cout << "\n=== TRYB BADAN ===\n";
    std::cout << "Algorytm: " << algorithmName << "\n";
    std::cout << "Struktura: " << structureName << "\n";
    std::cout << "Wariant: " << (variantName.empty() ? "-" : variantName) << "\n";
    std::cout << "Powtorzenia: " << repetitions << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    if (!CsvReportWriter::appendResult(csvFile,
                                       algorithmName,
                                       structureName,
                                       variantName.empty() ? "-" : variantName,
                                       repetitions,
                                       averageTime,
                                       minTime,
                                       maxTime)) {
        std::cout << "Nie udalo sie zapisac CSV.\n";
        return false;
    }

    std::cout << "Wynik zapisano do CSV: " << csvFile << "\n";
    return true;
}

// Uruchamia single
bool runSingleMode(const std::string& inputFile,
                   const std::string& structureName,
                   const std::string& algorithmName,
                   const std::string& variantName,
                   const std::string& outputFile,
                   bool saveOutput) {
    if (structureName == "array") {
        return runSingleForStructure<DynamicArray>(inputFile,
                                                   algorithmName,
                                                   variantName,
                                                   outputFile,
                                                   saveOutput);
    }

    if (structureName == "slist") {
        return runSingleForStructure<SinglyLinkedList>(inputFile,
                                                       algorithmName,
                                                       variantName,
                                                       outputFile,
                                                       saveOutput);
    }

    if (structureName == "dlist") {
        return runSingleForStructure<DoublyLinkedList>(inputFile,
                                                       algorithmName,
                                                       variantName,
                                                       outputFile,
                                                       saveOutput);
    }

    return false;
}

// Uruchamia research
bool runResearchMode(const std::string& inputFile,
                     int repetitions,
                     const std::string& structureName,
                     const std::string& algorithmName,
                     const std::string& variantName,
                     const std::string& csvFile) {
    if (structureName == "array") {
        return runResearchForStructure<DynamicArray>(inputFile,
                                                     repetitions,
                                                     algorithmName,
                                                     variantName,
                                                     csvFile,
                                                     structureName);
    }

    if (structureName == "slist") {
        return runResearchForStructure<SinglyLinkedList>(inputFile,
                                                         repetitions,
                                                         algorithmName,
                                                         variantName,
                                                         csvFile,
                                                         structureName);
    }

    if (structureName == "dlist") {
        return runResearchForStructure<DoublyLinkedList>(inputFile,
                                                         repetitions,
                                                         algorithmName,
                                                         variantName,
                                                         csvFile,
                                                         structureName);
    }

    return false;
}

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

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
        if (argc < 5) {
            std::cout << "Za malo argumentow dla trybu single.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];
        std::string structureName = argv[3];
        std::string algorithmName = argv[4];
        std::string variantName;
        std::string outputFile;
        bool saveOutput = false;

        if (!isValidStructureName(structureName)) {
            std::cout << "Niepoprawna struktura.\n";
            return 1;
        }

        if (!isValidAlgorithmName(algorithmName)) {
            std::cout << "Niepoprawny algorytm.\n";
            return 1;
        }

        // BucketSort nie ma wariantu
        if (algorithmName == "bucket") {
            if (argc >= 6) {
                outputFile = argv[5];
                saveOutput = true;
            }
        }

        // QuickSort i ShellSort mają wariant
        if (algorithmName == "quick" || algorithmName == "shell") {
            if (argc >= 6) {
                std::string fifthArgument = argv[5];

                if (!isValidVariantName(algorithmName, fifthArgument)) {
                    std::cout << "Niepoprawny wariant algorytmu.\n";
                    return 1;
                }

                variantName = fifthArgument;

                if (argc >= 7) {
                    outputFile = argv[6];
                    saveOutput = true;
                }
            }
        }

        return runSingleMode(inputFile,
                             structureName,
                             algorithmName,
                             variantName,
                             outputFile,
                             saveOutput) ? 0 : 1;
    }

    if (mode == "research") {
        if (argc < 7) {
            std::cout << "Za malo argumentow dla trybu research.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];
        int repetitions = 0;
        std::string structureName = argv[4];
        std::string algorithmName = argv[5];
        std::string csvFile = argv[6];
        std::string variantName;

        if (!parsePositiveInt(argv[3], repetitions)) {
            std::cout << "Niepoprawna liczba powtorzen.\n";
            return 1;
        }

        if (!isValidStructureName(structureName)) {
            std::cout << "Niepoprawna struktura.\n";
            return 1;
        }

        if (!isValidAlgorithmName(algorithmName)) {
            std::cout << "Niepoprawny algorytm.\n";
            return 1;
        }

        if (argc >= 8) {
            variantName = argv[7];

            if (!isValidVariantName(algorithmName, variantName)) {
                std::cout << "Niepoprawny wariant algorytmu.\n";
                return 1;
            }
        }

        return runResearchMode(inputFile,
                               repetitions,
                               structureName,
                               algorithmName,
                               variantName,
                               csvFile) ? 0 : 1;
    }

    std::cout << "Nieznany tryb: " << mode << "\n";
    showHelp();
    return 1;
}