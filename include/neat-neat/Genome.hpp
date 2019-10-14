#include<vector>

#ifndef GENOME_HPP
#define GENOME_HPP

#include "Connection.hpp"
#include "Node.hpp"

namespace neat_neat {

class Genome {
private:
    std::vector<std::shared_ptr<Connection>> connections;
    std::vector<std::shared_ptr<Node>> nodes;
    static size_t innovationNumber;
    double fitness;
    void randomIntersection(Genome);
    void mutateWeight();
    void mutateAddConnection();
    void mutateAddNode();
    
public:
    Genome();
    Genome(std::shared_ptr<Connection>&, std::shared_ptr<Connection>&);
    Genome crossover(Genome);
    void mutate();
    double compatibilityDistance();
    size_t putInnovationNumber();
    double evaluateFitness();
};

} //neat_neat

#endif