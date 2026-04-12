#ifndef QUICKSORTER_H
#define QUICKSORTER_H

#include <cstdlib>

// Wariant wyboru pivota
enum class QuickPivotStrategy {
    Middle,
    Random,
    Extreme
};

class QuickSorter {
private:
    // Zamienia miejscami dwa elementy
    template <typename Structure>
    static void swapValues(Structure& structure, int firstIndex, int secondIndex) {
        int temp = structure.get(firstIndex);
        structure.set(firstIndex, structure.get(secondIndex));
        structure.set(secondIndex, temp);
    }

    // Wybiera wartość pivota
    template <typename Structure>
    static int choosePivotValue(Structure& structure,
                                int left,
                                int right,
                                QuickPivotStrategy strategy) {
        if (strategy == QuickPivotStrategy::Random) {
            int randomIndex = left + std::rand() % (right - left + 1);
            return structure.get(randomIndex);
        }

        if (strategy == QuickPivotStrategy::Extreme) {
            return structure.get(left);
        }

        return structure.get((left + right) / 2);
    }

public:
    // Sortuje strukturę quicksortem
    template <typename Structure>
    static void sort(Structure& structure,
                     int left,
                     int right,
                     QuickPivotStrategy strategy = QuickPivotStrategy::Middle) {
        if (structure.getSize() == 0 || left >= right) {
            return;
        }

        int i = left;
        int j = right;
        int pivot = choosePivotValue(structure, left, right, strategy);

        while (i <= j) {
            while (structure.get(i) < pivot) {
                i++;
            }

            while (structure.get(j) > pivot) {
                j--;
            }

            if (i <= j) {
                swapValues(structure, i, j);
                i++;
                j--;
            }
        }

        if (left < j) {
            sort(structure, left, j, strategy);
        }

        if (i < right) {
            sort(structure, i, right, strategy);
        }
    }
};

#endif