#ifndef QUICKSORT_H
#define QUICKSORT_H

class QuickSort {
private:
    template <typename Structure>
    static void swapValues(Structure& structure, int firstIndex, int secondIndex) {
        int temp = structure.get(firstIndex);
        structure.set(firstIndex, structure.get(secondIndex));
        structure.set(secondIndex, temp);
    }

public:
    template <typename Structure>
    static void sort(Structure& structure, int left, int right) {
        if (structure.getSize() == 0 || left >= right) {
            return;
        }

        int i = left;
        int j = right;
        int pivot = structure.get((left + right) / 2);

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
            sort(structure, left, j);
        }

        if (i < right) {
            sort(structure, i, right);
        }
    }
};

#endif