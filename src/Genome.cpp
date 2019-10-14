#include <vector>
#include <iterator>
#include <random>
#include <algorithm>
#include <queue>
#include <memory>

#include "../include/neat-neat/Genome.hpp"
#include "../include/neat-neat/Node.hpp"

#define maxWeight 100
#define weightCoeff 0.4
#define excessCoeff 2
#define disjointCoeff 2 

namespace neat_neat {

size_t Genome::innovationNumber = 0;

Genome::Genome() {
    this->nodes.push_back(std::make_shared<Node> (sensor));
    this->nodes.push_back(std::make_shared<Node> (sensor));
    this->nodes.push_back(std::make_shared<Node> (output));
    auto randomWeight = std::bind(std::uniform_real_distribution(0, maxWeight), std::mt19937());    
    this->connections.push_back(std::make_shared<Connection> (nodes[0],nodes[2],
                                                            randomWeight(), 1, putInnovationNumber()));
    this->connections.push_back(std::make_shared<Connection> (nodes[1], nodes[2],
                                                            randomWeight(), 1, putInnovationNumber())); 
}

Genome::Genome(std::vector<std::shared_ptr<Connection>>& connections,
               std::vector<std::shared_ptr<Connection>>& nodes) {
    : connections(connections),
      nodes(nodes) {

}

void Genome::randomIntersection(Genome g, OutputIterator out) {
    auto select = std::bind(std::uniform_int_distribution(0, 1), std::mt19937());
    auto g1_it = this->connections.begin();
    auto g2_it = g.getConnections.begin();
    while(g1_it != this->connections.end() or g2_it != g.getConnections().end()) {
        if(*g1_it == *g2_it) {
            if(select()) {
                *out++ = *g1_it;
            }
            else {
                *out++ = *g2_it;
            }
            ++g1_it;
            ++g2_it;
        }
        else if(*g1_it > *g2_it) {
            ++g2_it;
        }
        else {
            ++g1_it;
        }
    }
}

Genome Genome::crossover(Genome g) { //TODO: specialize for equal fitness
    auto compareInnovation() = [&] (std::shared_ptr<Connection> c1,
                                    std::shared_ptr<Connection> c2) -> bool {
        return c1->getInnovationNumber() > c2->getInnovationNumber();
    };
    auto sortedGenome1 = std::sort(this->connections.begin(), this->connections.end(),
                                   compareInnovation()); 
    auto sortedGenome2 = std::sort(g.getConnection().begin(), g.getConnections.end(),
                                   compareInnovation());
    std::vector<std::shared_ptr<Connection> childConnections;
    childConnections.insert()
    if(this->fitness > g.getFitness()) {
        std::set_difference(sortedGenome1.begin(), sortedGenome1.end(),
                            sortedGenome2.begin(), sortedGenome2.end(),
                            std::back_insertor(childConnections));
    }
    else if(this->fitness() < g.getFitness()) {
        std::set_difference(sortedGenome2.begin(), sortedGenome2.end(),
                            sortedGenome1.begin(), sortedGenome1.end(),
                            std::back_insertor(childConnections));
    }
    std::set<std::shared_ptr<Node>> nodeSet;
    for(auto& connection : childConnections) {
        nodeSet.insert(connection->getIn());
        nodeSet.insert(connection->getOut());
    }
    for(auto& connection : childConnections) {
        connection->getIn()->getOutgoing().push_back(connection);
        connection->getOut()->getIncoming().push_back(connection);
    }
    std::vector<std::shared_ptr<Node>> childNodes;
    return Genome(childConnections, std::copy(nodeSet.begin(), nodeSet.end(),
                  std::back_inserter(childNodes)));
}


void Genome::mutateWeight() {
    auto select = std::bind(std::uniform_int_distribution<>(0, 1), std::mt19937());
    auto randomScalar = std::bind(std::uniform_real_distribution<>(0.5, 1.5), std::mt19937());
    for(auto connection : this->connections) {
        if(select()) {
            if(select()) {
                connection->setWeight(connection->getWeight() * randomScalar());
            }
            if(select()) {
                if(select()) {
                    connection->setWeight(connection->getWeight() - randomScalar());
                }
                else {
                    connection->setWeight(connection->getWeight() + randomScalar());
                }
            }
            if(select()) {
                connection->setWeight(connection->getWeight() * -1);
            }
        }
    }
}

void Genome::mutateAddConnection() {
    auto chooseRandomIndex = std::bind(std::uniform_int_distribution<>(0, this->nodes.size() -1),
                                        std::my19937());
    int randomIndex1 = chooseRandomIndex();
    std::shared_ptr<Node>& node1 = this->nodes.at(index1);
    int randomIndex2 = chooseRandomIndex();
    while(randomIndex2 < randomIndex1) {
        randomIndex2 = chooseRandomIndex();
    }
    std::shared_ptr<Node>& node2 = this->nodes.at(randomIndex2);
    auto randomWeight = std::bind(std::uniform_real_distribution(0, maxWeight), std::mt19937());
    auto enable() = std::bind(std::uniform_int_distribution(0, 1), std::mt19937());
    std::shared_ptr<Connection> newConnection = std::make_shared(Connection(node1, node2, 
                                                              randomWeight(), enable(),
                                                              putInnovationNumber()));
    this->connections.push_back(newConnection);
    node1->getOutgoing().push_back((newConnection));
    node2->getIncoming().push_back((newConnection));
}

void Genome::mutateAddNode() {
    auto chooseRandomIndex = std::bind
                                  (std::uniform_int_distribution<>(0, this->connections.size() -1),
                                   std::my19937());
    int randomIndex = chooseRandomIndex();
    std::shared_ptr<Connection> randomConnection = this->connections.at(randomIndex);
    randomConnection->disable();
    std::shared_ptr<Node>& preNode = randomConnection->getIn();
    std::shared_ptr<Node>& succNode = randomConnection->getOut();
    auto randomWeight = std::bind(std::uniform_real_distribution(0, maxWeight), std::mt19937());
    auto enable = std::bind(std::uniform_int_distribution(0, 1), std::mt19937());    
    std::shared_ptr<Connection> preConnection = std::make_shared(Connection(preNode, newNode,
                                                              randomWeight(), enable(),
                                                              putInnovationNumber()));
    std::shared_ptr<Connection> succConnection = std::make_shared(Connection(newNode, succNode,
                                                              1.0, enable(),
                                                              putInnovationNumber()));
    std::shared_ptr<Node> newNode = std::make_shared(Node(hidden));
    newNode->getIncoming().push_back(preConnection);
    newNode->outGoing().push_back(succConnection);
    this->node.push_back(newNode);
  }

void Genome::mutate() {
    auto select = std::bind(std::uniform_int_distribution(0, 1), std::mt19937());
    if(select()) {
        this->mutateWeight();
    }
    if(select()) {
        this->mutateAddConnection();
    }
    if(select()) {
        this->mutateAddNode();
    }
    auto compareInnovation() = [&] (std::shared_ptr<Connection> c1,
                                    std::shared_ptr<Connection> c2) -> bool {
        return c1->getInnovationNumber() > c2->getInnovationNumber();                                    
    };
    std::sort(this->connections.begin(), this->connections.end(), compareInnovation());
}

void Genome::putInnovationNumber() {
    return ++this->innovationNumber;
}

double Genome::compatibilityDistance(Genome g) {
    int max1 = this->connections.end()-1->getInnovationNumber(),
        max2 = g.getConnections().end()-1->getInnovationNumber(),
        excess = 0, disjoint = 0;
    std::vector<int> weightVector();
    int maxInnovationNumber = max1 > max2 ? max1 : max2;
    auto g1_it = this->connections.begin();
    auto g2_it = g.getConnections.begin();
    while(g1_it != this->connections.end() or g2_it != g.getConnections().end()) {
        if(*g1_it == *g2_it) {
            weightVector.push_back(g1_it->getWeight() - g2_it->getWeight());
            ++g1_it;
            ++g2_it;
        }
        else if(*g1_it > *g2_it) {
            if(*g1_it > maxInnovationNumber) {
                disjoint++;
            }
            else {
                excess++;
            }
            ++g2_it;
        }
        else {
            if(*g2_it > maxInnovationNumber) {
                disjoint++;
            }
            else {
                excess++;
            }
            ++g1_it;
        }
    }
    return (weightCoeff * std::accumulate(weightVector.begin(), weightVector.end(), 0.0)) +
           ((excessCoeff * excess) + (disjointCoeff * disjoint)) / std::max(this->nodes.size(),
                                                                            g.getNodes().size());
}

double Genome::networkOutput(std::pair<bool, bool> input) {
    std::set<std::shared_ptr<Node>> q;
    node[0]->setValue(input.first);
    for(auto& outgoing : node[0]->getOutgoing()) {
        q.insert(outgoing->getOut());
    }
    node[1]->setValue(input.second);
    for(auto& outgoing : node[1]->getOutgoing()) {
        q.insert(outgoing->getOut());
    }
    while(!q.empty()) {
        auto current = *q.begin();
        q.erase(q.begin());
        for(auto& outgoing : current->getOutgoing()) {
            q.insert(outgoing->getOut());
        }
        double value {0};
        for(auto& incoming : current->getIncoming()) {
            value += incoming->getWeight() * incoming->getIn()->getValue();
        }
    }
    return value;
}

double evaluateFitness() {
    std::unordered_map<std::pair<bool, bool>, bool> dataset = {{{true, true}, false},
                                                               {{true, false}, true},
                                                               {{false, true}, true},
                                                               {{false, false}, false}};
    double loss = 0;
    for(auto& datapoint : dataset) {
        loss += 1 - std::abs(networkOutput(datapoint.first) - datapoint(datapoint.second));
    }
    loss *= loss;
    this->fitness = loss;
    return loss;
}

} //neat_neat