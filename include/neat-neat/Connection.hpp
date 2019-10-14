#include <memory>
#include <cstddef>

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Node.hpp"

namespace neat_neat {

class Connection {
private:    
    std::shared_ptr<Node> in;
    std::shared_ptr<Node> out;
    double weight;
    bool enabled;
    size_t innovationNumber;

public:
    Connection(std::shared_ptr<Node>&, std::shared_ptr<Node>&, double, bool, size_t);
    std::shared_ptr<Node>& getIn();
    std::shared_ptr<Node>& getOut();
    double getWeight();
    bool isEnabled();
    size_t getInnovationNumber();
    void setWeight(double);
    void enable();
    void disable();
};

} //neat_neat

#endif