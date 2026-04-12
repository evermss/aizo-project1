#ifndef SHELLSORTER_H
#define SHELLSORTER_H

// Wariant ciągu odstępów
enum class ShellGapStrategy {
    Halving,
    Knuth
};

class ShellSorter {
public:
    // Sortuje strukturę shellem
    template <typename Structure>
    static void sort(Structure& structure,
                     ShellGapStrategy strategy = ShellGapStrategy::Halving) {
        int size = structure.getSize();

        if (size <= 1) {
            return;
        }

        if (strategy == ShellGapStrategy::Halving) {
            // Odstępy: n/2, n/4, ...
            for (int gap = size / 2; gap > 0; gap /= 2) {
                for (int i = gap; i < size; i++) {
                    int currentValue = structure.get(i);
                    int j = i;

                    while (j >= gap && structure.get(j - gap) > currentValue) {
                        structure.set(j, structure.get(j - gap));
                        j -= gap;
                    }

                    structure.set(j, currentValue);
                }
            }
        } else {
            // Odstępy Knutha
            int gap = 1;
            while (gap < size / 3) {
                gap = 3 * gap + 1;
            }

            while (gap > 0) {
                for (int i = gap; i < size; i++) {
                    int currentValue = structure.get(i);
                    int j = i;

                    while (j >= gap && structure.get(j - gap) > currentValue) {
                        structure.set(j, structure.get(j - gap));
                        j -= gap;
                    }

                    structure.set(j, currentValue);
                }

                gap = (gap - 1) / 3;
            }
        }
    }
};

#endif