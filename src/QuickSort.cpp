#include "QuickSort.h"

void QuickSort::sort(DynamicArray& arr, int left, int right) {
    int i = left;
    int j = right;
    int pivot = arr[(left + right) / 2];

    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j) {
        QuickSort::sort(arr, left, j);
    }

    if (i < right) {
        QuickSort::sort(arr, i, right);
    }
}