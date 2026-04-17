#include <iostream>
#include <chrono>

#include "Parameters.h"
#include "FileLoader.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "structures/DoublyLinkedList.h"
#include "structures/Stack.h"
#include "structures/LinearStructure.h"
#include "algorithms/QuickSorter.h"
#include "algorithms/ShellSorter.h"
#include "algorithms/BucketSorter.h"
#include "algorithms/SortChecker.h"
#include "research/ResearchRunner.h"

void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Biblioteka parametrow: " << Parameters::getVersion() << "\n\n";

    std::cout << "Obslugiwane algorytmy:\n";
    std::cout << "  quick\n";
    std::cout << "  shell\n";
    std::cout << "  bucket\n\n";

    std::cout << "Obslugiwane struktury w trybie single:\n";
    std::cout << "  array\n";
    std::cout << "  singleList\n";
    std::cout << "  doubleList\n";
    std::cout << "  stack\n\n";

    std::cout << "Struktura binaryTree jest uzywana tylko w badaniu omega.\n\n";
}

void printStructure(const LinearStructure& structure) {
    for (int i = 0; i < structure.getSize(); i++) {
        std::cout << structure.get(i) << " ";
    }
    std::cout << "\n";
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

template <typename Structure>
bool sortSingleStructure(Structure& structure) {
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

// Wczytuje dane, sortuje i opcjonalnie zapisuje wynik do pliku.
template <typename Structure>
bool runSingleForStructure() {
    Structure structure;

    if (!FileLoader::loadFromFile(Parameters::inputFile, structure)) {
        std::cout << "Blad wczytywania pliku.\n";
        return false;
    }

    std::cout << "Dane przed sortowaniem:\n";
    printStructure(structure);

    const auto startTime = std::chrono::high_resolution_clock::now();

    if (structure.getSize() > 0) {
        if (!sortSingleStructure(structure)) {
            std::cout << "Nieobslugiwany algorytm.\n";
            return false;
        }
    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto duration =
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
        std::cout << "binaryTree nie jest obslugiwane w trybie single.\n";
        std::cout << "Drzewo jest uzywane tylko w badaniu omega.\n";
        return false;
    }

    if (Parameters::structure == Parameters::Structures::stack) {
        return runSingleForStructure<Stack>();
    }

    std::cout << "Nieobslugiwana struktura.\n";
    return false;
}

int main(int argc, char** argv) {
    const int result = Parameters::readParameters(argc - 1, argv + 1);

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