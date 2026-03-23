#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include "DynamicArray.h"

class FileLoader {
public:
    static bool loadFromFile(const std::string& fileName, DynamicArray& arr);
    static bool saveToFile(const std::string& fileName, const DynamicArray& arr);
};

#endif