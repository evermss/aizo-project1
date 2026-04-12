#ifndef BUCKETSORTER_H
#define BUCKETSORTER_H

#include "../structures/LinearStructure.h"
#include "../structures/DynamicArray.h"

class BucketSorter {
private:
    // Sortuje pojedynczy kubełek
    static void insertionSort(DynamicArray& bucket);

public:
    // Sortuje strukturę kubełkowo
    static void sort(LinearStructure& structure, int bucketCount = 10);
};

#endif