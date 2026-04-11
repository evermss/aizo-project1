#include "CsvReportWriter.h"
#include <fstream>
#include <ctime>
#include <iomanip>

// Dopisuje jeden wynik do CSV
bool CsvReportWriter::appendResult(const std::string& fileName,
                                   const std::string& algorithmName,
                                   const std::string& structureName,
                                   const std::string& variantName,
                                   int repetitions,
                                   double averageTime,
                                   long long minTime,
                                   long long maxTime) {
    bool fileExists = false;

    {
        std::ifstream testFile(fileName);
        fileExists = testFile.good();
    }

    std::ofstream outputFile(fileName, std::ios::app);

    if (!outputFile.is_open()) {
        return false;
    }

    // Dodaje nagłówek tylko raz
    if (!fileExists) {
        outputFile << "date,time,algorithm,structure,variant,repetitions,average_us,min_us,max_us\n";
    }

    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    outputFile << std::put_time(localTime, "%Y-%m-%d") << ",";
    outputFile << std::put_time(localTime, "%H:%M:%S") << ",";
    outputFile << algorithmName << ",";
    outputFile << structureName << ",";
    outputFile << variantName << ",";
    outputFile << repetitions << ",";
    outputFile << averageTime << ",";
    outputFile << minTime << ",";
    outputFile << maxTime << "\n";

    return true;
}