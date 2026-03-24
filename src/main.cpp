#include <iostream>
#include <chrono>
#include <limits>
#include <string>
#include "FileLoader.h"
#include "DynamicArray.h"
#include "QuickSort.h"
#include "SortChecker.h"

// Wyswietla instrukcje obslugi programu
void showHelp() {
    std::cout << "=== AiZO Projekt 1 ===\n";
    std::cout << "Dostepne tryby:\n";
    std::cout << "  help\n";
    std::cout << "  single <plik_wejsciowy> [plik_wyjsciowy]\n";
    std::cout << "  research <plik_wejsciowy> <powtorzenia>\n\n";

    std::cout << "Przyklady:\n";
    std::cout << "  ./aizo-project1 help\n";
    std::cout << "  ./aizo-project1 single ../data.txt\n";
    std::cout << "  ./aizo-project1 single ../data.txt ../sorted.txt\n";
    std::cout << "  ./aizo-project1 research ../data.txt 10\n\n";

    std::cout << "Format pliku wejsciowego:\n";
    std::cout << "  1 linia - liczba elementow\n";
    std::cout << "  kolejne linie - wartosci\n";
}

// Wypisuje wszystkie elementy tablicy
void printArray(const DynamicArray& arr) {
    for (int i = 0; i < arr.getSize(); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

// Sprawdza, czy tekst jest poprawna dodatnia liczba calkowita
bool parsePositiveInt(const std::string& line, int& value) {
    value = std::stoi(line);
    return value > 0;
}

// Tryb pojedynczego testu
bool runSingleMode(const std::string& inputFile, const std::string& outputFile, bool saveOutput) {
    DynamicArray arr;

    // Wczytanie danych z pliku do tablicy
    if (!FileLoader::loadFromFile(inputFile, arr)) {
        std::cout << "Blad wczytywania pliku.\n";
        return false;
    }

    std::cout << "Dane przed sortowaniem:\n";
    printArray(arr);

    // Pomiar czasu dotyczy tylko samego sortowania
    auto start = std::chrono::high_resolution_clock::now();

    if (arr.getSize() > 0) {
        QuickSort::sort(arr, 0, arr.getSize() - 1);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Dane po sortowaniu:\n";
    printArray(arr);

    // Sprawdzenie, czy dane zostaly poprawnie posortowane
    if (SortChecker::isSorted(arr)) {
        std::cout << "Tablica jest poprawnie posortowana.\n";
    } else {
        std::cout << "Blad sortowania.\n";
        return false;
    }

    std::cout << "Czas sortowania: " << duration.count() << " mikrosekund\n";

    // Opcjonalny zapis wyniku do pliku
    if (saveOutput) {
        if (FileLoader::saveToFile(outputFile, arr)) {
            std::cout << "Zapisano wynik do pliku: " << outputFile << "\n";
        } else {
            std::cout << "Nie udalo sie zapisac wyniku.\n";
            return false;
        }
    }

    return true;
}

// Tryb badan - wielokrotne uruchomienie sortowania
bool runResearchMode(const std::string& inputFile, int repetitions) {
    if (repetitions <= 0) {
        std::cout << "Liczba powtorzen musi byc wieksza od 0.\n";
        return false;
    }

    long long sum = 0;
    long long minTime = std::numeric_limits<long long>::max();
    long long maxTime = std::numeric_limits<long long>::min();

    std::cout << "=== POMIARY ===\n";

    for (int i = 0; i < repetitions; i++) {
        // W kazdej iteracji tworzymy nowa tablice
        DynamicArray arr;

        // W kazdej iteracji od nowa wczytujemy dane z pliku
        if (!FileLoader::loadFromFile(inputFile, arr)) {
            std::cout << "Blad wczytywania pliku.\n";
            return false;
        }

        // Start pomiaru czasu
        auto start = std::chrono::high_resolution_clock::now();

        if (arr.getSize() > 0) {
            QuickSort::sort(arr, 0, arr.getSize() - 1);
        }

        // Koniec pomiaru czasu
        auto end = std::chrono::high_resolution_clock::now();

        long long duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "Pomiar " << i + 1 << ": " << duration << " mikrosekund\n";

        // Sprawdzenie poprawnosci po kazdym sortowaniu
        if (!SortChecker::isSorted(arr)) {
            std::cout << "Blad sortowania w powtorzeniu: " << i + 1 << "\n";
            return false;
        }

        sum += duration;

        if (duration < minTime) {
            minTime = duration;
        }

        if (duration > maxTime) {
            maxTime = duration;
        }
    }

    // Obliczenie sredniego czasu z wszystkich pomiarow
    double average = static_cast<double>(sum) / repetitions;

    std::cout << "\n=== TRYB BADAN ===\n";
    std::cout << "Plik: " << inputFile << "\n";
    std::cout << "Powtorzenia: " << repetitions << "\n";
    std::cout << "Algorytm: QuickSort\n";
    std::cout << "Sredni czas: " << average << " mikrosekund\n";
    std::cout << "Min czas: " << minTime << " mikrosekund\n";
    std::cout << "Max czas: " << maxTime << " mikrosekund\n";

    return true;
}

int main(int argc, char* argv[]) {
    // Gdy nie podano argumentow, pokazujemy pomoc
    if (argc < 2) {
        showHelp();
        return 0;
    }

    std::string mode = argv[1];

    // Tryb pomocy
    if (mode == "help") {
        showHelp();
        return 0;
    }

    // Tryb pojedynczego testu
    if (mode == "single") {
        if (argc < 3) {
            std::cout << "Brak pliku wejsciowego.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];

        if (argc >= 4) {
            std::string outputFile = argv[3];
            return runSingleMode(inputFile, outputFile, true) ? 0 : 1;
        }

        return runSingleMode(inputFile, "", false) ? 0 : 1;
    }

    // Tryb badan
    if (mode == "research") {
        if (argc < 4) {
            std::cout << "Brak wymaganych argumentow do trybu research.\n";
            showHelp();
            return 1;
        }

        std::string inputFile = argv[2];
        int repetitions = 0;

        if (!parsePositiveInt(argv[3], repetitions)) {
            std::cout << "Niepoprawna liczba powtorzen.\n";
            return 1;
        }

        return runResearchMode(inputFile, repetitions) ? 0 : 1;
    }

    // Nieznany tryb programu
    std::cout << "Nieznany tryb: " << mode << "\n";
    showHelp();
    return 1;
}