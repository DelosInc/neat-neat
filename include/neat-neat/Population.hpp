#include <vector>
#include <cstddef>

#ifndef POPULATION_H
#define POPULATION_H

#include "../neat-neat/Species.hpp"

namespace neat_neat {

class Population {
    private:
        std::vector<Species> species;
        void speciate(Genome entering);
        std::vector<double> speciesAdjustedFitness();
        size_t populationSize();
        std::vector<size_t> newSpeciesSize(std::vector<double> speciesAdjustedFitness,
                                           double populationAdjustedFitness);
        std::vector<Genome> representatives();
        std::vector<Species> assignSpecies(std::vector<Genome> offspring,
                                           std::vector<Genome> representative);
        Genome fittestGenome();

    public:
        Population(const size_t size);
        void reproduce();
}

} //neat_neat

#endif