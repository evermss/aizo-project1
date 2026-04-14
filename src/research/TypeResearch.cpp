#include "TypeResearch.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "../include/Parameters.h"
#include "../structures/TemplateArray.h"
#include "../algorithms/TemplateQuickSort.h"
#include "../algorithms/TemplateSortChecker.h"
#include "../CsvReportWriter.h"

namespace {
    // Generator liczb losowych uzywany w badaniu typow danych.
    static std::mt19937 rng(std::random_device{}());

    // Zwraca losowa liczbe unsigned int z pelnego zakresu typu.
    unsigned int randomUnsignedInt() {
        static std::uniform_int_distribution<unsigned int> dist(
            std::numeric_limits<unsigned int>::min(),
            std::numeric_limits<unsigned int>::max()
        );
        return dist(rng);
    }

    // Zwraca losowa liczbe typu double z wybranego zakresu.
    double randomDouble() {
        static std::uniform_real_distribution<double> dist(
            -1000000.0,
            1000000.0
        );
        return dist(rng);
    }

    std::string randomString() {
        static std::uniform_int_distribution<int> lenDist(5, 12);
        static std::uniform_int_distribution<int> charDist(32, 126);

        const int length = lenDist(rng);
        std::string value;
        value.reserve(length);

        for (int i = 0; i < length; i++) {
            value += static_cast<char>(charDist(rng));
        }

        return value;
    }

    template <typename T>
    void fillArray(TemplateArray<T>& array, int size);

    // Wypelnia tablice losowymi wartosciami typu unsigned int.
    template <>
    void fillArray<unsigned int>(TemplateArray<unsigned int>& array, int size) {
        for (int i = 0; i < size; i++) {
            array.pushBack(randomUnsignedInt());
        }
    }

    // Wypelnia tablice losowymi wartosciami typu double.
    template <>
    void fillArray<double>(TemplateArray<double>& array, int size) {
        for (int i = 0; i < size; i++) {
            array.pushBack(randomDouble());
        }
    }

    // Wypelnia tablice losowymi napisami.
    template <>
    void fillArray<std::string>(TemplateArray<std::string>& array, int size) {
        for (int i = 0; i < size; i++) {
            array.pushBack(randomString());
        }
    }

    // Wykonuje badanie dla jednego typu danych: generuje
    template <typename T>
    bool runTypeCase(const std::string& typeName,
                     const std::string& csvFile,
                     int size,
                     int repetitions) {
        if (size <= 0 || repetitions <= 0) {
            std::cout << "Niepoprawne parametry badania typu.\n";
            return false;
        }

        long long sum = 0;
        long long minTime = std::numeric_limits<long long>::max();
        long long maxTime = std::numeric_limits<long long>::min();

        for (int i = 0; i < repetitions; i++) {
            TemplateArray<T> array;
            fillArray<T>(array, size);

            const auto startTime = std::chrono::high_resolution_clock::now();

            if (array.getSize() > 0) {
                TemplateQuickSort::sort(array, 0, array.getSize() - 1, TemplatePivotStrategy::Middle);
            }

            const auto endTime = std::chrono::high_resolution_clock::now();

            const long long duration =
                std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

            if (!TemplateSortChecker::isSorted(array)) {
                std::cout << "Blad sortowania dla typu: " << typeName << "\n";
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

        std::cout << "Typ: " << typeName << "\n";
        std::cout << "Rozmiar: " << size << "\n";
        std::cout << "Powtorzenia: " << repetitions << "\n";
        std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
        std::cout << "Min czas: " << minTime << " mikrosekund\n";
        std::cout << "Max czas: " << maxTime << " mikrosekund\n\n";

        return CsvReportWriter::appendResult(
            csvFile,
            "quick",
            "template_array_" + typeName,
            "middle",
            repetitions,
            averageTime,
            minTime,
            maxTime
        );
    }
}

// Uruchamia badanie C, czyli porownanie czasu sortowania
bool TypeResearch::run() {
    const std::string csvFile = Parameters::resultsFile.empty()
        ? "C_results.csv"
        : Parameters::resultsFile;

    const int size = Parameters::structureSize;
    const int repetitions = Parameters::iterations;

    bool ok = true;

    ok = runTypeCase<double>("double", csvFile, size, repetitions) && ok;
    ok = runTypeCase<unsigned int>("unsigned_int", csvFile, size, repetitions) && ok;
    ok = runTypeCase<std::string>("string", csvFile, size, repetitions) && ok;

    return ok;
}