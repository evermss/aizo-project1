# AiZO Projekt 1

Projekt z przedmiotu **Algorytmy i Złożoność Obliczeniowa**.

Program umożliwia:
- sortowanie danych z pliku,
- uruchamianie badań porównawczych,
- zapis wyników do pliku CSV.

## Zaimplementowane algorytmy
- Quick Sort
- Shell Sort
- Bucket Sort

## Zaimplementowane struktury danych
- Dynamic Array
- Singly Linked List
- Doubly Linked List
- Stack
- Binary Search Tree

## Uwagi o strukturach
W trybie `single` i w zwykłych badaniach wykorzystywane są liniowe struktury danych:
- array
- singleList
- doubleList
- stack

`binaryTree` jest używane tylko w badaniu **omega** jako struktura nieliniowa.

## Wymagania
- C++17
- CMake
- biblioteka prowadzącego `libparameters.so`

## Budowanie projektu

```bash
cmake -S . -B build
cmake --build build

```text
5
76
-142
0
2138
-4