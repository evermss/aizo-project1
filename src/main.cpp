#include <iostream>
#include <chrono>
#include <limits>
#include <string>
#include <random>

#include "Parameters.h"
#include "FileLoader.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "structures/DoublyLinkedList.h"
#include "structures/BinarySearchTree.h"
#include "structures/Stack.h"
#include "algorithms/QuickSorter.h"
#include "algorithms/ShellSorter.h"
#include "algorithms/BucketSorter.h"
#include "algorithms/SortChecker.h"
#include "structures/LinearStructure.h"
#include "CsvReportWriter.h"
#include "research/ResearchRunner.h"


namespace {
    std::mt19937 rng(std::random_device{}());

    int randomInt() {
        static std::uniform_int_distribution<int> dist(0, 1000000);
        return dist(rng);
    }
}

// Pokazuje pomoc
void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Biblioteka parametrow: " << Parameters::getVersion() << "\n\n";

    std::cout << "Obslugiwane algorytmy:\n";
    std::cout << "  quick\n";
    std::cout << "  shell\n";
    std::cout << "  bucket\n\n";

    std::cout << "Obslugiwane struktury:\n";
    std::cout << "  array\n";
    std::cout << "  singleList\n";
    std::cout << "  doubleList\n";
    std::cout << "  binaryTree\n";
    std::cout << "  stack\n\n";
}

// Wypisuje strukture
void printStructure(const LinearStructure& structure) {
    for (int i = 0; i < structure.getSize(); i++) {
        std::cout << structure.get(i) << " ";
    }
    std::cout << "\n";
}

// Mapuje pivot
QuickPivotStrategy mapPivot(Parameters::Pivots pivot) {
    if (pivot == Parameters::Pivots::random) {
        return QuickPivotStrategy::Random;
    }

    if (pivot == Parameters::Pivots::left || pivot == Parameters::Pivots::right) {
        return QuickPivotStrategy::Extreme;
    }

    return QuickPivotStrategy::Middle;
}

// Mapuje parametr shella
ShellGapStrategy mapShellParameter(Parameters::ShellParameters shellParameter) {
    if (shellParameter == Parameters::ShellParameters::option2 ||
        shellParameter == Parameters::ShellParameters::option3 ||
        shellParameter == Parameters::ShellParameters::option4) {
        return ShellGapStrategy::Knuth;
    }

    return ShellGapStrategy::Halving;
}

// Zwraca nazwe algorytmu
std::string getAlgorithmName(Parameters::Algorithms algorithm) {
    if (algorithm == Parameters::Algorithms::quick) {
        return "quick";
    }

    if (algorithm == Parameters::Algorithms::shell) {
        return "shell";
    }

    if (algorithm == Parameters::Algorithms::bucket) {
        return "bucket";
    }

    return "unknown";
}

// Zwraca nazwe struktury
std::string getStructureName(Parameters::Structures structure) {
    if (structure == Parameters::Structures::array) {
        return "array";
    }

    if (structure == Parameters::Structures::singleList) {
        return "slist";
    }

    if (structure == Parameters::Structures::doubleList) {
        return "dlist";
    }

    if (structure == Parameters::Structures::binaryTree) {
        return "binaryTree";
    }

    if (structure == Parameters::Structures::stack) {
        return "stack";
    }

    return "unknown";
}

// Zwraca nazwe wariantu
std::string getVariantName() {
    if (Parameters::algorithm == Parameters::Algorithms::quick) {
        if (Parameters::pivot == Parameters::Pivots::random) {
            return "random";
        }

        if (Parameters::pivot == Parameters::Pivots::left) {
            return "left";
        }

        if (Parameters::pivot == Parameters::Pivots::right) {
            return "right";
        }

        return "middle";
    }

    if (Parameters::algorithm == Parameters::Algorithms::shell) {
        if (Parameters::shellParameter == Parameters::ShellParameters::option2) {
            return "option2";
        }

        if (Parameters::shellParameter == Parameters::ShellParameters::option3) {
            return "option3";
        }

        if (Parameters::shellParameter == Parameters::ShellParameters::option4) {
            return "option4";
        }

        return "option1";
    }

    return "-";
}

// Sortuje strukture
template <typename Structure>
bool sortStructure(Structure& structure) {
    if (Parameters::algorithm == Parameters::Algorithms::quick) {
        QuickPivotStrategy strategy = mapPivot(Parameters::pivot);
        QuickSorter::sort(structure, 0, structure.getSize() - 1, strategy);
        return true;
    }

    if (Parameters::algorithm == Parameters::Algorithms::shell) {
        ShellGapStrategy strategy = mapShellParameter(Parameters::shellParameter);
        ShellSorter::sort(structure, strategy);
        return true;
    }

    if (Parameters::algorithm == Parameters::Algorithms::bucket) {
        BucketSorter::sort(structure);
        return true;
    }

    return false;
}

// Dodaje losowe dane
template <typename Structure>
void fillStructureWithRandomInts(Structure& structure, int size) {
    structure.clear();

    for (int i = 0; i < size; i++) {
        structure.pushBack(randomInt());
    }
}


// Tryb single dla struktury
template <typename Structure>
bool runSingleForStructure() {
    Structure structure;

    if (!FileLoader::loadFromFile(Parameters::inputFile, structure)) {
        std::cout << "Blad wczytywania pliku.\n";
        return false;
    }

    std::cout << "Dane przed sortowaniem:\n";
    printStructure(structure);

    auto startTime = std::chrono::high_resolution_clock::now();

    if (structure.getSize() > 0) {
        if (!sortStructure(structure)) {
            std::cout << "Nieobslugiwany algorytm.\n";
            return false;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    std::cout << "Dane po sortowaniu:\n";
    printStructure(structure);

    if (!SortChecker::isSorted(structure)) {
        std::cout << "Blad sortowania.\n";
        return false;
    }

    std::cout << "Sortowanie poprawne.\n";
    std::cout << "Czas sortowania: " << duration.count() << " mikrosekund\n";

    if (!Parameters::outputFile.empty()) {
        if (!FileLoader::saveToFile(Parameters::outputFile, structure)) {
            std::cout << "Nie udalo sie zapisac pliku.\n";
            return false;
        }

        std::cout << "Zapisano wynik do pliku: " << Parameters::outputFile << "\n";
    }

    return true;
}

// Tryb research dla struktury
template <typename Structure>
bool runResearchForStructure() {
    int repetitions = Parameters::iterations;
    int size = Parameters::structureSize;

    if (repetitions <= 0) {
        std::cout << "Niepoprawna liczba powtorzen.\n";
        return false;
    }

    if (size <= 0) {
        std::cout << "Niepoprawny rozmiar struktury.\n";
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        Structure structure;

        fillStructureWithRandomInts(structure, size);

        auto startTime = std::chrono::high_resolution_clock::now();

        if (structure.getSize() > 0) {
            if (!sortStructure(structure)) {
                std::cout << "Nieobslugiwany algorytm.\n";
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
    std::cout << "Algorytm: " << getAlgorithmName(Parameters::algorithm) << "\n";
    std::cout << "Struktura: " << getStructureName(Parameters::structure) << "\n";
    std::cout << "Wariant: " << getVariantName() << "\n";
    std::cout << "Rozmiar: " << size << "\n";
    std::cout << "Powtorzenia: " << repetitions << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    if (!Parameters::resultsFile.empty()) {
        if (!CsvReportWriter::appendResult(Parameters::resultsFile,
                                           getAlgorithmName(Parameters::algorithm),
                                           getStructureName(Parameters::structure),
                                           getVariantName(),
                                           repetitions,
                                           averageTime,
                                           minTime,
                                           maxTime)) {
            std::cout << "Nie udalo sie zapisac CSV.\n";
            return false;
        }

        std::cout << "Wynik zapisano do CSV: " << Parameters::resultsFile << "\n";
    }

    return true;
}

// Uruchamia single
bool runSingleMode() {
    if (Parameters::structure == Parameters::Structures::array) {
        return runSingleForStructure<DynamicArray>();
    }

    if (Parameters::structure == Parameters::Structures::singleList) {
        return runSingleForStructure<SinglyLinkedList>();
    }

    if (Parameters::structure == Parameters::Structures::doubleList) {
        return runSingleForStructure<DoublyLinkedList>();
    }

    if (Parameters::structure == Parameters::Structures::binaryTree) {
        return runSingleForStructure<BinarySearchTree>();
    }

    if (Parameters::structure == Parameters::Structures::stack) {
        return runSingleForStructure<Stack>();
    }

    std::cout << "Nieobslugiwana struktura.\n";
    return false;
}

// Uruchamia research
bool runResearchMode() {
    if (Parameters::structure == Parameters::Structures::array) {
        return runResearchForStructure<DynamicArray>();
    }

    if (Parameters::structure == Parameters::Structures::singleList) {
        return runResearchForStructure<SinglyLinkedList>();
    }

    if (Parameters::structure == Parameters::Structures::doubleList) {
        return runResearchForStructure<DoublyLinkedList>();
    }

    if (Parameters::structure == Parameters::Structures::binaryTree) {
        return runResearchForStructure<BinarySearchTree>();
    }

    if (Parameters::structure == Parameters::Structures::stack) {
        return runResearchForStructure<Stack>();
    }

    std::cout << "Nieobslugiwana struktura.\n";
    return false;
}

int main(int argc, char** argv) {
    int result = Parameters::readParameters(argc - 1, argv + 1);

    if (result != 0) {
        std::cout << "Blad odczytu parametrow.\n";
        Parameters::help();
        return 1;
    }

    if (Parameters::runMode == Parameters::RunModes::help) {
        Parameters::help();
        return 0;
    }

    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        return runSingleMode() ? 0 : 1;
    }

    if (Parameters::runMode == Parameters::RunModes::benchmark) {
        return ResearchRunner::runSelectedResearch() ? 0 : 1;
    }

    showHelp();
    return 1;
}