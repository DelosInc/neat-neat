#include <vector>
#include <cstddef>
#include <numeric>
#include <unordered_map>

#include "../neat-neat/Population.hpp"

#define compatibilityThreshold 1

namespace neat_neat {

Population::Population(const size_t size) {
    for(size_t _ = size; _--;) {
        speciate(new Genome());
    }
}

void Population::speciate(Genome entering) {
    for(auto& species : this->species) {
        if(species.size()) {
            Genome existing = species.randomGenome();
            if(entering.compatibilityDistance(existing) <= compatibilityThreshold) {    //TODO: add config
                species.addGenome(entering);
                return;
            }
        }
    }
    species.push_back(new Species(entering));
}

void Population::reproduce() {
    while(true) {
        auto speciesAdjustedFitness = this->speciesAdjustedFitness();
        auto populationAdjustedFitness = std::accumulate(speciesAdjustedFitness.begin(),
                                                        speciesAdjustedFitness.end(), 0.0)
                                                        / populationSize();
        auto newSpeciesSize = this->newSpeciesSize(speciesAdjustedFitness,
                                                populationAdjustedFitness);
        auto representatives = this->representatives();
        std::vector<Genome> offspring;
        for(size_t i = 0; i < species.size(); ++i) {
            species[i].eliminateWeakGenomes();
            auto speciesOffspring = species[i].reproduce(newSpeciesSize[i]);
            offspring.insert(offspring.end(), speciesOffspring.begin(), speciesOffspring.end());
        }
        species = std::move(speciate(offspring, representatives));
        Genome fittest = this->fittestGenome();
    }
}

std::vector<double> Population::speciesAdjustedFitness() {
    std::vector<double> speciesAdjustedFitness;
    for(auto& species : this->species) {
        speciesAdjustedFitness.push_back(species.adjustedFitness());
    }
    return speciesAdjustedFitness;
}

size_t Population::populationSize() {
    size_t populationSize {0};
    for(auto& species : this->species) {
        populationSize += species.size();
    }
}

std::vector<size_t> Population::newSpeciesSize(std::vector<double> speciesAdjustedFitness,
                                               double populationAdjustedFitness) {
    std::vector<size_t> newSpeciesSize;
    for(auto& speciesAdjustedFitness: speciesAdjustedFitness) {
        newSpeciesSize.push_back(std::static_cast<size_t>(std::round(
                                 speciesAdjustedFitness / populationAdjustedFitness)));
    }
    return newSpeciesSize;
}

std::vector<Genome> Population::representatives() {
    std::vector<Genome> representatives;
    for(auto& species : this->species) {
        representatives.push_back(species.randomGenome());
    }
}

std::vector<Species> Population::assignSpecies(std::vector<Genome> offspring,
                                               std::vector<Genome> representatives) {
    std::unordered_map<Genome, Species> map;
    std::vector<Species> outliers;
    for(auto& offspring : offspring) {
        for(auto& representative : representatives) {
            if(offspring.compatibilityDistance(representative) <= compatibilityThreshold) {
                if(map.contains(representative)) {
                    map[representative].addGenome(offspring);
                }
                else {
                    map[representative] = new Species(offspring);
                }
                break;
            }
        }
        outliers.push_back(new Species(offspring));
    }
    auto species = std::move(outliers);
    for(auto& map : map) {
        species.push_back(map->second);
    }
    return species;
}

} //neat_neat