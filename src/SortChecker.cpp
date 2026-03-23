#include "SortChecker.h"

bool SortChecker::isSorted(const DynamicArray& arr) {
    for (int i = 0; i < arr.getSize() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }

    return true;
}