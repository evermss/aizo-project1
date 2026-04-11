#ifndef BUCKETSORTER_H
#define BUCKETSORTER_H

#include "LinearStructure.h"
#include "DynamicArray.h"

class BucketSorter {
private:
    // Sortuje pojedynczy kubełek
    static void insertionSort(DynamicArray& bucket);

public:
    // Sortuje strukturę kubełkowo
    static void sort(LinearStructure& structure, int bucketCount = 10);
};

#endif