#include <vector>
#include <cstddef>

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include "../neat-neat/Genome.hpp"

namespace neat_neat {

class Species {
    private:
        std::vector<Genome> genomes;

    public:
        Species(Genome genome);
        Genome randomGenome();
        void addGenome(Genome entering);
        float adjustedFitness();
        void eliminateWeakGenomes();
        std::vector<Genome> reproduce(size_t newSpeciesSize);
        size_t size();
        Genome fittestGenome();
}

} //neat_neat

#endif