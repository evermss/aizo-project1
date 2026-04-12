#include "algorithms/SortChecker.h"

// Sprawdza, czy struktura jest posortowana rosnaco
bool SortChecker::isSorted(const LinearStructure& structure) {
    for (int i = 0; i < structure.getSize() - 1; i++) {
        if (structure.get(i) > structure.get(i + 1)) {
            return false;
        }
    }

    return true;
}