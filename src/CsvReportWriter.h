#ifndef CSVREPORTWRITER_H
#define CSVREPORTWRITER_H

#include <string>

class CsvReportWriter {
public:
    // Dopisuje wynik do pliku CSV
    static bool appendResult(const std::string& fileName,
                             const std::string& algorithmName,
                             const std::string& structureName,
                             const std::string& variantName,
                             int repetitions,
                             double averageTime,
                             long long minTime,
                             long long maxTime);
};

#endif