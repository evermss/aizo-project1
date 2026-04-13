#ifndef RESEARCHRUNNER_H
#define RESEARCHRUNNER_H

class ResearchRunner {
public:
    static bool runAlphaResearch();
    static bool runSizeResearch();
    static bool runDistributionResearch();
    static bool runTypeResearch();
    static bool runOmegaResearch();
    static bool runSelectedResearch();

private:
    static bool runSingleBenchmarkCase();
};

#endif