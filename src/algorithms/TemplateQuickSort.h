#ifndef TEMPLATEQUICKSORT_H
#define TEMPLATEQUICKSORT_H

#include <random>

enum class TemplatePivotStrategy {
    Random,
    Middle
};

class TemplateQuickSort {
private:
    static int randomPivotIndex(int left, int right) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(left, right);
        return dist(rng);
    }

    static int choosePivotIndex(int left, int right, TemplatePivotStrategy strategy) {
        if (strategy == TemplatePivotStrategy::Random) {
            return randomPivotIndex(left, right);
        }

        return (left + right) / 2;
    }

public:
    template <typename ArrayType>
    static void sort(ArrayType& array, int left, int right, TemplatePivotStrategy strategy) {
        if (left >= right) {
            return;
        }

        int i = left;
        int j = right;
        int pivotIndex = choosePivotIndex(left, right, strategy);
        auto pivot = array[pivotIndex];

        while (i <= j) {
            while (array[i] < pivot) {
                i++;
            }

            while (array[j] > pivot) {
                j--;
            }

            if (i <= j) {
                auto temp = array[i];
                array[i] = array[j];
                array[j] = temp;
                i++;
                j--;
            }
        }

        if (left < j) {
            sort(array, left, j, strategy);
        }

        if (i < right) {
            sort(array, i, right, strategy);
        }
    }
};

#endif