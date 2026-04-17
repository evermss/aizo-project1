#include "CsvReportWriter.h"

#include <ctime>
#include <fstream>
#include <iomanip>

#include "Parameters.h"

namespace {

std::string getDataTypeName() {
    if (Parameters::dataType == Parameters::DataTypes::typeInt) {
        return "int";
    }

    if (Parameters::dataType == Parameters::DataTypes::typeFloat) {
        return "float";
    }

    if (Parameters::dataType == Parameters::DataTypes::typeDouble) {
        return "double";
    }

    if (Parameters::dataType == Parameters::DataTypes::typeChar) {
        return "char";
    }

    if (Parameters::dataType == Parameters::DataTypes::typeString) {
        return "string";
    }

    if (Parameters::dataType == Parameters::DataTypes::tyleUnsignedInt) {
        return "unsigned_int";
    }

    if (Parameters::dataType == Parameters::DataTypes::typeUnsignedLong) {
        return "unsigned_long";
    }

    if (Parameters::dataType == Parameters::DataTypes::typeUnsignedChar) {
        return "unsigned_char";
    }

    return "undefined";
}

std::string getDistributionName() {
    if (Parameters::distribution == Parameters::Distribution::random) {
        return "random";
    }

    if (Parameters::distribution == Parameters::Distribution::ascending) {
        return "ascending";
    }

    if (Parameters::distribution == Parameters::Distribution::descending) {
        return "descending";
    }

    if (Parameters::distribution == Parameters::Distribution::ascending50Per) {
        return "ascending50Per";
    }

    return "undefined";
}

std::string getPivotName() {
    if (Parameters::pivot == Parameters::Pivots::random) {
        return "random";
    }

    if (Parameters::pivot == Parameters::Pivots::left) {
        return "left";
    }

    if (Parameters::pivot == Parameters::Pivots::right) {
        return "right";
    }

    if (Parameters::pivot == Parameters::Pivots::middle) {
        return "middle";
    }

    return "undefined";
}

std::string getShellName() {
    if (Parameters::shellParameter == Parameters::ShellParameters::option1) {
        return "option1";
    }

    if (Parameters::shellParameter == Parameters::ShellParameters::option2) {
        return "option2";
    }

    if (Parameters::shellParameter == Parameters::ShellParameters::option3) {
        return "option3";
    }

    if (Parameters::shellParameter == Parameters::ShellParameters::option4) {
        return "option4";
    }

    return "undefined";
}

}

bool CsvReportWriter::appendResult(const std::string& fileName,
                                   const std::string& algorithmName,
                                   const std::string& structureName,
                                   const std::string& variantName,
                                   int size,
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

    if (!fileExists) {
        outputFile << "date,time,algorithm,structure,variant,size,repetitions,data_type,distribution,pivot,shell_gap,average_us,min_us,max_us\n";
    }

    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    outputFile << std::put_time(localTime, "%Y-%m-%d") << ",";
    outputFile << std::put_time(localTime, "%H:%M:%S") << ",";
    outputFile << algorithmName << ",";
    outputFile << structureName << ",";
    outputFile << variantName << ",";
    outputFile << size << ",";
    outputFile << repetitions << ",";
    outputFile << getDataTypeName() << ",";
    outputFile << getDistributionName() << ",";
    outputFile << getPivotName() << ",";
    outputFile << getShellName() << ",";
    outputFile << averageTime << ",";
    outputFile << minTime << ",";
    outputFile << maxTime << "\n";

    return true;
}