#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include "LinearStructure.h"

class FileLoader {
public:
    static bool loadFromFile(const std::string& fileName, LinearStructure& structure);
    static bool saveToFile(const std::string& fileName, const LinearStructure& structure);
};

#endif