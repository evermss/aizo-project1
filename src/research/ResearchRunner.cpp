#include "ResearchRunner.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "../include/Parameters.h"
#include "../CsvReportWriter.h"
#include "../algorithms/BucketSorter.h"
#include "../algorithms/QuickSorter.h"
#include "../algorithms/ShellSorter.h"
#include "../algorithms/SortChecker.h"
#include "../structures/BinarySearchTree.h"
#include "../structures/DoublyLinkedList.h"
#include "../structures/DynamicArray.h"
#include "../structures/SinglyLinkedList.h"
#include "../structures/Stack.h"
#include "TypeResearch.h"

namespace {

enum class DataLayout {
    Random,
    Ascending,
    Descending,
    HalfSorted
};

DataLayout currentLayout = DataLayout::Random;

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

std::string getLayoutName(DataLayout layout) {
    if (layout == DataLayout::Random) {
        return "random";
    }

    if (layout == DataLayout::Ascending) {
        return "ascending";
    }

    if (layout == DataLayout::Descending) {
        return "descending";
    }

    return "half_sorted";
}

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

    return getLayoutName(currentLayout);
}

template <typename Structure>
void fillStructure(Structure& structure, int size, DataLayout layout) {
    structure.clear();

    if (layout == DataLayout::Random) {
        for (int i = 0; i < size; i++) {
            structure.pushBack(randomIntFullRange());
        }
        return;
    }

    if (layout == DataLayout::Ascending) {
        for (int i = 0; i < size; i++) {
            structure.pushBack(i);
        }
        return;
    }

    if (layout == DataLayout::Descending) {
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

bool fileNameContains(const std::string& text) {
    return Parameters::resultsFile.find(text) != std::string::npos;
}

}  // namespace

bool ResearchRunner::runSingleBenchmarkCase() {
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
            duration = measureCase<BinarySearchTree>();
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

bool ResearchRunner::runAlphaResearch() {
    bool allOk = true;

    const Parameters::Pivots originalPivot = Parameters::pivot;
    const Parameters::ShellParameters originalShellParameter = Parameters::shellParameter;

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

    return allOk;
}

bool ResearchRunner::runSizeResearch() {
    const int originalSize = Parameters::structureSize;

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
    return allOk;
}

bool ResearchRunner::runDistributionResearch() {
    const DataLayout layouts[] = {
        DataLayout::Random,
        DataLayout::Descending,
        DataLayout::Ascending,
        DataLayout::HalfSorted
    };

    bool allOk = true;

    for (const DataLayout layout : layouts) {
        currentLayout = layout;

        if (!runSingleBenchmarkCase()) {
            allOk = false;
        }
    }

    currentLayout = DataLayout::Random;
    return allOk;
}

bool ResearchRunner::runTypeResearch() {
    return TypeResearch::run();
}

bool ResearchRunner::runOmegaResearch() {
    const Parameters::Structures originalStructure = Parameters::structure;
    bool allOk = true;

    Parameters::structure = Parameters::Structures::array;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = Parameters::Structures::singleList;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = Parameters::Structures::doubleList;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = Parameters::Structures::binaryTree;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = Parameters::Structures::stack;
    allOk = runSingleBenchmarkCase() && allOk;

    Parameters::structure = originalStructure;
    return allOk;
}

bool ResearchRunner::runSelectedResearch() {
    if (fileNameContains("alpha")) {
        return runAlphaResearch();
    }

    if (fileNameContains("omega")) {
        return runOmegaResearch();
    }

    if (fileNameContains("C_") || fileNameContains("C.")) {
        return runTypeResearch();
    }

    if (fileNameContains("B_") || fileNameContains("B.")) {
        return runDistributionResearch();
    }

    if (fileNameContains("A_") || fileNameContains("A.")) {
        return runSizeResearch();
    }

    if (fileNameContains("type")) {
        return runTypeResearch();
    }

    if (fileNameContains("distribution")) {
        return runDistributionResearch();
    }

    if (fileNameContains("size")) {
        return runSizeResearch();
    }

    std::cout << "Nie udalo sie rozpoznac badania z nazwy pliku wynikowego.\n";
    std::cout << "Uzyj np. alpha_results.csv, A_results.csv, B_results.csv, C_results.csv albo omega_results.csv.\n";
    return false;
}