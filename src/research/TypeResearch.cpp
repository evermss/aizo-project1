#include "TypeResearch.h"
#include <iostream>
#include <chrono>
#include <limits>
#include <string>
#include <cstdlib>
#include <random>

#include "../structures/TemplateArray.h"
#include "../algorithms/TemplateQuickSort.h"
#include "../algorithms/TemplateSortChecker.h"
#include "../CsvReportWriter.h"

static std::mt19937 rng(std::random_device{}());

int randomInt() {
    static std::uniform_int_distribution<int> dist(0, 1000000);
    return dist(rng);
}

unsigned int randomUnsignedInt() {
    static std::uniform_int_distribution<unsigned int> dist(0U, 1000000U);
    return dist(rng);
}

std::string randomString() {
    static std::uniform_int_distribution<int> letterDist(0, 25);

    std::string value;
    value += static_cast<char>('A' + letterDist(rng));
    value += static_cast<char>('A' + letterDist(rng));
    value += static_cast<char>('A' + letterDist(rng));
    value += static_cast<char>('A' + letterDist(rng));
    value += static_cast<char>('A' + letterDist(rng));

    return value;
}

template <typename T>
bool runTypeCase(const std::string& typeName,
                 const std::string& csvFile,
                 int size,
                 int repetitions) {
    if (size <= 0 || repetitions <= 0) {
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        TemplateArray<T> array;

        for (int j = 0; j < size; j++) {
            array.pushBack(T{});
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        TemplateQuickSort::sort(array, 0, array.getSize() - 1, TemplatePivotStrategy::Middle);
        auto endTime = std::chrono::high_resolution_clock::now();

        long long duration =
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

    double averageTime = static_cast<double>(sum) / repetitions;

    std::cout << "Typ: " << typeName << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n\n";

    return CsvReportWriter::appendResult(csvFile,
                                         "quick",
                                         "template_array_" + typeName,
                                         "middle",
                                         repetitions,
                                         averageTime,
                                         minTime,
                                         maxTime);
}

template <>
bool runTypeCase<int>(const std::string& typeName,
                      const std::string& csvFile,
                      int size,
                      int repetitions) {
    if (size <= 0 || repetitions <= 0) {
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        TemplateArray<int> array;

        for (int j = 0; j < size; j++) {
            array.pushBack(randomInt());
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        TemplateQuickSort::sort(array, 0, array.getSize() - 1, TemplatePivotStrategy::Middle);
        auto endTime = std::chrono::high_resolution_clock::now();

        long long duration =
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

    double averageTime = static_cast<double>(sum) / repetitions;

    std::cout << "Typ: " << typeName << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n\n";

    return CsvReportWriter::appendResult(csvFile,
                                         "quick",
                                         "template_array_" + typeName,
                                         "middle",
                                         repetitions,
                                         averageTime,
                                         minTime,
                                         maxTime);
}

template <>
bool runTypeCase<unsigned int>(const std::string& typeName,
                               const std::string& csvFile,
                               int size,
                               int repetitions) {
    if (size <= 0 || repetitions <= 0) {
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        TemplateArray<unsigned int> array;

        for (int j = 0; j < size; j++) {
            array.pushBack(static_cast<unsigned int>(std::rand()));
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        TemplateQuickSort::sort(array, 0, array.getSize() - 1, TemplatePivotStrategy::Middle);
        auto endTime = std::chrono::high_resolution_clock::now();

        long long duration =
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

    double averageTime = static_cast<double>(sum) / repetitions;

    std::cout << "Typ: " << typeName << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n\n";

    return CsvReportWriter::appendResult(csvFile,
                                         "quick",
                                         "template_array_" + typeName,
                                         "middle",
                                         repetitions,
                                         averageTime,
                                         minTime,
                                         maxTime);
}

template <>
bool runTypeCase<std::string>(const std::string& typeName,
                              const std::string& csvFile,
                              int size,
                              int repetitions) {
    if (size <= 0 || repetitions <= 0) {
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    for (int i = 0; i < repetitions; i++) {
        TemplateArray<std::string> array;

        for (int j = 0; j < size; j++) {
            std::string value;
            value += static_cast<char>('A' + std::rand() % 26);
            value += static_cast<char>('A' + std::rand() % 26);
            value += static_cast<char>('A' + std::rand() % 26);
            value += static_cast<char>('A' + std::rand() % 26);
            value += static_cast<char>('A' + std::rand() % 26);
            array.pushBack(value);
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        TemplateQuickSort::sort(array, 0, array.getSize() - 1, TemplatePivotStrategy::Middle);
        auto endTime = std::chrono::high_resolution_clock::now();

        long long duration =
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

    double averageTime = static_cast<double>(sum) / repetitions;

    std::cout << "Typ: " << typeName << "\n";
    std::cout << "Sredni czas: " << averageTime << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n\n";

    return CsvReportWriter::appendResult(csvFile,
                                         "quick",
                                         "template_array_" + typeName,
                                         "middle",
                                         repetitions,
                                         averageTime,
                                         minTime,
                                         maxTime);
}

bool TypeResearch::run() {
    const std::string csvFile = "C_results.csv";
    const int size = 10000;
    const int repetitions = 20;

    bool ok = true;

    ok = ok && runTypeCase<int>("int", csvFile, size, repetitions);
    ok = ok && runTypeCase<unsigned int>("unsigned_int", csvFile, size, repetitions);
    ok = ok && runTypeCase<std::string>("string", csvFile, size, repetitions);

    return ok;
}