#include "Timer.h"

// Rozpoczyna pomiar czasu.
void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

// Konczy pomiar czasu.
void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
}

// Zwraca czas trwania pomiaru w mikrosekundach.
long long Timer::getDurationInMicroseconds() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}