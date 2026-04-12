#ifndef TEMPLATESORTCHECKER_H
#define TEMPLATESORTCHECKER_H

class TemplateSortChecker {
public:
    template <typename ArrayType>
    static bool isSorted(const ArrayType& array) {
        for (int i = 0; i < array.getSize() - 1; i++) {
            if (array[i] > array[i + 1]) {
                return false;
            }
        }

        return true;
    }
};

#endif