#include "ResearchRunner.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "Parameters.h"
#include "CsvReportWriter.h"
#include "algorithms/BucketSorter.h"
#include "algorithms/QuickSorter.h"
#include "algorithms/ShellSorter.h"
#include "algorithms/SortChecker.h"
#include "structures/BinarySearchTree.h"
#include "structures/DoublyLinkedList.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "structures/Stack.h"
#include "TypeResearch.h"

namespace {

Parameters::Distribution currentLayout = Parameters::Distribution::random;
std::mt19937 rng(std::random_device{}());

int randomIntFullRange() {
    static std::uniform_int_distribution<int> dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    );
    return dist(rng);
}

QuickPivotStrategy mapPivot(Parameters::Pivots pivot) {
    if (pivot == Parameters::Pivots::random) {
        return QuickPivotStrategy::Random;
    }

    if (pivot == Parameters::Pivots::left || pivot == Parameters::Pivots::right) {
        return QuickPivotStrategy::Extreme;
    }

    return QuickPivotStrategy::Middle;
}

ShellGapStrategy mapShellParameter(Parameters::ShellParameters shellParameter) {
    if (shellParameter == Parameters::ShellParameters::option2 ||
        shellParameter == Parameters::ShellParameters::option3 ||
        shellParameter == Parameters::ShellParameters::option4) {
        return ShellGapStrategy::Knuth;
    }

    return ShellGapStrategy::Halving;
}

std::string getAlgorithmName() {
    if (Parameters::algorithm == Parameters::Algorithms::quick) {
        return "quick";
    }

    if (Parameters::algorithm == Parameters::Algorithms::shell) {
        return "shell";
    }

    if (Parameters::algorithm == Parameters::Algorithms::bucket) {
        return "bucket";
    }

    return "unknown";
}

std::string getStructureName() {
    if (Parameters::structure == Parameters::Structures::array) {
        return "array";
    }

    if (Parameters::structure == Parameters::Structures::singleList) {
        return "single_list";
    }

    if (Parameters::structure == Parameters::Structures::doubleList) {
        return "double_list";
    }

    if (Parameters::structure == Parameters::Structures::binaryTree) {
        return "binary_tree";
    }

    if (Parameters::structure == Parameters::Structures::stack) {
        return "stack";
    }

    return "unknown";
}

std::string getLayoutName(Parameters::Distribution layout) {
    if (layout == Parameters::Distribution::random) {
        return "random";
    }

    if (layout == Parameters::Distribution::ascending) {
        return "ascending";
    }

    if (layout == Parameters::Distribution::descending) {
        return "descending";
    }

    if (layout == Parameters::Distribution::ascending50Per) {
        return "ascending50Per";
    }

    return "undefined";
}

bool fileNameContains(const std::string& text) {
    return Parameters::resultsFile.find(text) != std::string::npos;
}

std::string getVariantName() {
    if (Parameters::distribution != Parameters::Distribution::undefined) {
        return getLayoutName(Parameters::distribution);
    }

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

    return getLayoutName(currentLayout);
}

template <typename Structure>
void fillStructure(Structure& structure, int size, Parameters::Distribution layout) {
    structure.clear();

    if (layout == Parameters::Distribution::random) {
        for (int i = 0; i < size; i++) {
            structure.pushBack(randomIntFullRange());
        }
        return;
    }

    if (layout == Parameters::Distribution::ascending) {
        for (int i = 0; i < size; i++) {
            structure.pushBack(i);
        }
        return;
    }

    if (layout == Parameters::Distribution::descending) {
        for (int i = size; i > 0; i--) {
            structure.pushBack(i);
        }
        return;
    }

    const int half = size / 2;

    for (int i = 0; i < half; i++) {
        structure.pushBack(i);
    }

    for (int i = half; i < size; i++) {
        structure.pushBack(randomIntFullRange());
    }
}

template <typename Structure>
bool sortStructure(Structure& structure) {
    if (Parameters::algorithm == Parameters::Algorithms::quick) {
        const QuickPivotStrategy strategy = mapPivot(Parameters::pivot);
        QuickSorter::sort(structure, 0, structure.getSize() - 1, strategy);
        return true;
    }

    if (Parameters::algorithm == Parameters::Algorithms::shell) {
        const ShellGapStrategy strategy = mapShellParameter(Parameters::shellParameter);
        ShellSorter::sort(structure, strategy);
        return true;
    }

    if (Parameters::algorithm == Parameters::Algorithms::bucket) {
        BucketSorter::sort(structure);
        return true;
    }

    return false;
}

template <typename Structure>
long long measureCase() {
    Structure structure;
    fillStructure(structure, Parameters::structureSize, currentLayout);

    const auto startTime = std::chrono::high_resolution_clock::now();

    if (structure.getSize() > 0) {
        if (!sortStructure(structure)) {
            std::cout << "Nieobslugiwany algorytm.\n";
            return -1;
        }
    }

    const auto endTime = std::chrono::high_resolution_clock::now();

    if (!SortChecker::isSorted(structure)) {
        std::cout << "Blad sortowania.\n";
        return -1;
    }

    return std::chrono::duration_cast<std::chrono::microseconds>(
               endTime - startTime
           ).count();
}

// Drzewo mierzymy osobno tylko w omega.
long long measureBinaryTreeCase() {
    BinarySearchTree tree;
    const int size = Parameters::structureSize;

    const auto startTime = std::chrono::high_resolution_clock::now();

    fillStructure(tree, size, currentLayout);

    const auto endTime = std::chrono::high_resolution_clock::now();

    if (!SortChecker::isSorted(tree)) {
        std::cout << "Blad drzewa binarnego.\n";
        return -1;
    }

    return std::chrono::duration_cast<std::chrono::microseconds>(
               endTime - startTime
           ).count();
}

bool saveBinaryTreeOmegaResult() {
    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < Parameters::iterations; i++) {
        long long duration = measureBinaryTreeCase();

        if (duration < 0) {
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

    const double averageTime = static_cast<double>(sum) / Parameters::iterations;

    std::cout << "\n=== TRYB BADAN ===\n";
    std::cout << "Algorytm: tree_build\n";
    std::cout << "Struktura: binary_tree\n";
    std::cout << "Wariant: omega\n";
    std::cout << "Rozmiar: " << Parameters::structureSize << "\n";
    std::cout << "Powtorzenia: " << Parameters::iterations << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    if (!Parameters::resultsFile.empty()) {
        if (!CsvReportWriter::appendResult(
                Parameters::resultsFile,
                "tree_build",
                "binary_tree",
                "omega",
                Parameters::structureSize,
                Parameters::iterations,
                averageTime,
                minTime,
                maxTime)) {
            std::cout << "Nie udalo sie zapisac CSV dla binaryTree.\n";
            return false;
        }

        std::cout << "Wynik zapisano do CSV: " << Parameters::resultsFile << "\n";
    }

    return true;
}

}

// Wykonuje jeden przypadek badawczy wiele razy i zapisuje wynik.
bool ResearchRunner::runSingleBenchmarkCase() {
    if (Parameters::distribution != Parameters::Distribution::undefined) {
        currentLayout = Parameters::distribution;
    }

    const int repetitions = Parameters::iterations;
    const int size = Parameters::structureSize;

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
        long long duration = -1;

        if (Parameters::structure == Parameters::Structures::array) {
            duration = measureCase<DynamicArray>();
        } else if (Parameters::structure == Parameters::Structures::singleList) {
            duration = measureCase<SinglyLinkedList>();
        } else if (Parameters::structure == Parameters::Structures::doubleList) {
            duration = measureCase<DoublyLinkedList>();
        } else if (Parameters::structure == Parameters::Structures::binaryTree) {
            std::cout << "binaryTree jest przeznaczone tylko do badania omega.\n";
            return false;
        } else if (Parameters::structure == Parameters::Structures::stack) {
            duration = measureCase<Stack>();
        } else {
            std::cout << "Nieobslugiwana struktura.\n";
            return false;
        }

        if (duration < 0) {
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

    const double averageTime = static_cast<double>(sum) / repetitions;

    std::cout << "\n=== TRYB BADAN ===\n";
    std::cout << "Algorytm: " << getAlgorithmName() << "\n";
    std::cout << "Struktura: " << getStructureName() << "\n";
    std::cout << "Wariant: " << getVariantName() << "\n";
    std::cout << "Rozmiar: " << size << "\n";
    std::cout << "Powtorzenia: " << repetitions << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    if (!Parameters::resultsFile.empty()) {
        if (!CsvReportWriter::appendResult(
                Parameters::resultsFile,
                getAlgorithmName(),
                getStructureName(),
                getVariantName(),
                size,
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

// Badanie alpha: quick testuje pivoty, shell testuje warianty odstepow.
bool ResearchRunner::runAlphaResearch() {
    bool allOk = true;

    const Parameters::Pivots originalPivot = Parameters::pivot;
    const Parameters::ShellParameters originalShellParameter = Parameters::shellParameter;
    const Parameters::Distribution originalDistribution = Parameters::distribution;

    Parameters::distribution = Parameters::Distribution::undefined;

    if (Parameters::algorithm == Parameters::Algorithms::quick) {
        Parameters::pivot = Parameters::Pivots::random;
        allOk = runSingleBenchmarkCase() && allOk;

        Parameters::pivot = Parameters::Pivots::middle;
        allOk = runSingleBenchmarkCase() && allOk;

        Parameters::pivot = Parameters::Pivots::left;
        allOk = runSingleBenchmarkCase() && allOk;
    } else if (Parameters::algorithm == Parameters::Algorithms::shell) {
        Parameters::shellParameter = Parameters::ShellParameters::option1;
        allOk = runSingleBenchmarkCase() && allOk;

        Parameters::shellParameter = Parameters::ShellParameters::option2;
        allOk = runSingleBenchmarkCase() && allOk;
    } else {
        std::cout << "Badanie alpha dotyczy quick albo shell.\n";
        allOk = false;
    }

    Parameters::pivot = originalPivot;
    Parameters::shellParameter = originalShellParameter;
    Parameters::distribution = originalDistribution;

    return allOk;
}

// Badanie A: porownanie czasu dla roznych rozmiarow danych.
bool ResearchRunner::runSizeResearch() {
    const int originalSize = Parameters::structureSize;
    const Parameters::Distribution originalDistribution = Parameters::distribution;

    Parameters::distribution = Parameters::Distribution::undefined;
    currentLayout = Parameters::Distribution::random;

    const int sizes[] = {
        originalSize / 4,
        originalSize / 2,
        originalSize,
        originalSize * 2
    };

    bool allOk = true;

    for (const int size : sizes) {
        if (size <= 0) {
            continue;
        }

        Parameters::structureSize = size;

        if (!runSingleBenchmarkCase()) {
            allOk = false;
        }
    }

    Parameters::structureSize = originalSize;
    Parameters::distribution = originalDistribution;

    if (originalDistribution != Parameters::Distribution::undefined) {
        currentLayout = originalDistribution;
    } else {
        currentLayout = Parameters::Distribution::random;
    }

    return allOk;
}

// Badanie B: porownanie roznych ukladow danych wejsciowych.
bool ResearchRunner::runDistributionResearch() {
    const Parameters::Distribution originalDistribution = Parameters::distribution;
    bool allOk = true;

    Parameters::distribution = Parameters::Distribution::random;
    currentLayout = Parameters::distribution;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::distribution = Parameters::Distribution::descending;
    currentLayout = Parameters::distribution;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::distribution = Parameters::Distribution::ascending;
    currentLayout = Parameters::distribution;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::distribution = Parameters::Distribution::ascending50Per;
    currentLayout = Parameters::distribution;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::distribution = originalDistribution;

    if (originalDistribution != Parameters::Distribution::undefined) {
        currentLayout = originalDistribution;
    } else {
        currentLayout = Parameters::Distribution::random;
    }

    return allOk;
}

// Badanie C: porownanie dla roznych typow danych.
bool ResearchRunner::runTypeResearch() {
    return TypeResearch::run();
}

// Badanie omega: porownanie algorytmu na kilku strukturach.
bool ResearchRunner::runOmegaResearch() {
    const Parameters::Structures originalStructure = Parameters::structure;
    const Parameters::Distribution originalDistribution = Parameters::distribution;
    bool allOk = true;

    Parameters::distribution = Parameters::Distribution::undefined;
    currentLayout = Parameters::Distribution::random;

    Parameters::structure = Parameters::Structures::array;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = Parameters::Structures::singleList;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = Parameters::Structures::doubleList;
    allOk = runSingleBenchmarkCase() && allOk;

    allOk = saveBinaryTreeOmegaResult() && allOk;

    Parameters::structure = Parameters::Structures::stack;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = originalStructure;
    Parameters::distribution = originalDistribution;

    if (originalDistribution != Parameters::Distribution::undefined) {
        currentLayout = originalDistribution;
    } else {
        currentLayout = Parameters::Distribution::random;
    }

    return allOk;
}

// Najpierw sprawdza parametry, a potem awaryjnie nazwe pliku.
bool ResearchRunner::runSelectedResearch() {
    if (Parameters::distribution != Parameters::Distribution::undefined) {
        currentLayout = Parameters::distribution;
        return runSingleBenchmarkCase();
    }

    if (Parameters::algorithm == Parameters::Algorithms::quick &&
        Parameters::pivot != Parameters::Pivots::undefined) {
        return runSingleBenchmarkCase();
    }

    if (Parameters::algorithm == Parameters::Algorithms::shell &&
        Parameters::shellParameter != Parameters::ShellParameters::undefined) {
        return runSingleBenchmarkCase();
    }

    if (fileNameContains("alpha")) {
        return runAlphaResearch();
    }

    if (fileNameContains("omega")) {
        return runOmegaResearch();
    }

    if (fileNameContains("C_") || fileNameContains("C.") || fileNameContains("type")) {
        return runTypeResearch();
    }

    if (fileNameContains("B_") || fileNameContains("B.") || fileNameContains("distribution")) {
        return runDistributionResearch();
    }

    if (fileNameContains("A_") || fileNameContains("A.") || fileNameContains("size")) {
        return runSizeResearch();
    }

    std::cout << "Nie udalo sie rozpoznac badania.\n";
    return false;
}