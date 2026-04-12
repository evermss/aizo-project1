#include "algorithms/BucketSorter.h"

// Sortuje jeden kubełek
void BucketSorter::insertionSort(DynamicArray& bucket) {
    for (int i = 1; i < bucket.getSize(); i++) {
        int currentValue = bucket[i];
        int j = i - 1;

        while (j >= 0 && bucket[j] > currentValue) {
            bucket[j + 1] = bucket[j];
            j--;
        }

        bucket[j + 1] = currentValue;
    }
}

// Sortuje strukturę kubełkowo
void BucketSorter::sort(LinearStructure& structure, int bucketCount) {
    int size = structure.getSize();

    if (size <= 1) {
        return;
    }

    if (bucketCount <= 0) {
        bucketCount = 10;
    }

    int minValue = structure.get(0);
    int maxValue = structure.get(0);

    // Szuka minimum i maksimum
    for (int i = 1; i < size; i++) {
        int value = structure.get(i);

        if (value < minValue) {
            minValue = value;
        }

        if (value > maxValue) {
            maxValue = value;
        }
    }

    if (minValue == maxValue) {
        return;
    }

    DynamicArray* buckets = new DynamicArray[bucketCount];

    long long range = static_cast<long long>(maxValue)
                    - static_cast<long long>(minValue)
                    + 1;

    // Rozrzuca wartości do kubełków
    for (int i = 0; i < size; i++) {
        int value = structure.get(i);

        long long shiftedValue =
            static_cast<long long>(value) - static_cast<long long>(minValue);

        int bucketIndex = static_cast<int>((shiftedValue * bucketCount) / range);

        if (bucketIndex >= bucketCount) {
            bucketIndex = bucketCount - 1;
        }

        buckets[bucketIndex].pushBack(value);
    }

    // Sortuje kubełki
    for (int i = 0; i < bucketCount; i++) {
        insertionSort(buckets[i]);
    }

    // Składa wynik z kubełków
    int resultIndex = 0;

    for (int i = 0; i < bucketCount; i++) {
        for (int j = 0; j < buckets[i].getSize(); j++) {
            structure.set(resultIndex, buckets[i][j]);
            resultIndex++;
        }
    }

    delete[] buckets;
}