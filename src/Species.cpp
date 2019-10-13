#include <random>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>

#include <../neat-neat/Species.hpp>

#define r 60

namespace neat_neat {

Species::Species(Genome genome) {
    genomes.push_back(genome);
}

Genome Species::randomGenome() {
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::uniform_int_distribution<int> distribution(0, genomes.size() - 1);
    return genomes.at(distribution(random_engine));
}

void Species::addGenome(Genome entering) {
    genomes.push_back(entering);
}

double Species::adjustedFitness() {
    double fitness {0};
    for(auto &genome : this->genomes) {
        fitness += genome.fitness();
    }
    return fitness / genomes.size();
}

void Species::eliminateWeakGenomes() {
    std::sort(genomes.begin(), genomes.end(), std::greater<Genome>());
    std::erase(std::advance(genomes.begin(), std::static_cast<size_t>(std::round(
                            (r * genomes.size() ) / 100 + 1 ))), genomes.end());  //TODO: get from config
}

std::vector<Genome> Species::reproduce(size_t newSpeciesSize) {
    std::vector<Genome> offspring;
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::uniform_int_distribution<int> distribution(0, genomes.size() - 1);
    for(size_t _ = newSpeciesSize; _--;) {
        offspring.push_back(genomes.at(distribution(random_engine)).crossover(
                            genomes.at(distribution(random_engine))));
    }
    for(auto& offspring : offspring) {
        offspring.mutate();
    }
    return offspring;
}

size_t Species::size() {
    return genomes.size();
}

Genome Species::fittestGenome() {
    return genomes.at(i);
}

} //neat_neat