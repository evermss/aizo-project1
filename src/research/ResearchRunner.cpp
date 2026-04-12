#include "ResearchRunner.h"
#include <iostream>
#include "../include/Parameters.h"
#include "TypeResearch.h"

// Badanie alpha
bool ResearchRunner::runAlphaResearch() {
    std::cout << "Uruchomiono badanie alpha.\n";
    return true;
}

// Badanie A
bool ResearchRunner::runSizeResearch() {
    std::cout << "Uruchomiono badanie A.\n";
    return true;
}

// Badanie B
bool ResearchRunner::runDistributionResearch() {
    std::cout << "Uruchomiono badanie B.\n";
    return true;
}

// Badanie C
bool ResearchRunner::runTypeResearch() {
    std::cout << "Uruchomiono badanie C.\n";
    return true;
}

// Badanie Omega
bool ResearchRunner::runOmegaResearch() {
    std::cout << "Uruchomiono badanie Omega.\n";
    return true;
}

// Wybiera odpowiednie badanie
bool ResearchRunner::runSelectedResearch() {
    if (Parameters::algorithm == Parameters::Algorithms::quick &&
     Parameters::structure == Parameters::Structures::array &&
     Parameters::dataType == Parameters::DataTypes::typeString) {
        std::cout << "Uruchomiono badanie C.\n";
        return TypeResearch::run();
     }

    if (Parameters::algorithm == Parameters::Algorithms::quick &&
        Parameters::structure == Parameters::Structures::array &&
        Parameters::dataType == Parameters::DataTypes::typeInt) {
        std::cout << "Uruchomiono badanie B.\n";
        return runDistributionResearch();
        }

    return false;
}