#ifndef CSVREPORTWRITER_H
#define CSVREPORTWRITER_H

#include <string>

class CsvReportWriter {
public:
    static bool appendResult(const std::string& fileName,
                         const std::string& algorithmName,
                         const std::string& structureName,
                         const std::string& variantName,
                         int size,
                         int repetitions,
                         double averageTime,
                         long long minTime,
                         long long maxTime);
};

#endif