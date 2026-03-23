#include "QuickSort.h"

void QuickSort::sort(DynamicArray& arr, int left, int right) {
    // Koniec rekurencji: pusta tablica albo jeden element
    if (arr.getSize() == 0 || left >= right) {
        return;
    }

    int i = left;
    int j = right;
    int pivot = arr[(left + right) / 2]; // Wybor pivota ze srodka

    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        // Zamiana elementow po niewlasciwej stronie pivota
        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

    // Rekurencyjne sortowanie lewej czesci
    if (left < j) {
        sort(arr, left, j);
    }

    // Rekurencyjne sortowanie prawej czesci
    if (i < right) {
        sort(arr, i, right);
    }
}