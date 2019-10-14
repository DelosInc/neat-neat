#include <cstddef>
#include <vector>
#include <memory>

#ifndef NODE_GENE_HPP
#define NODE_GENE_HPP

#include <../neat-neat/Connection.hpp>

namespace neat_neat {

enum NodeType {
    sensor,
    output,
    hidden
};

class Node {
    private:
        const NodeType type;
        double value;
        std::vector<std::shared_ptr<Connection>> incoming;
        std::vector<std::shared_ptr<Connection>> outgoing;

    public:
        Node(NodeType type);
        size_t getId();
        NodeType getType();
        double getValue();
        void setValue(double value);
        std::vector<std::shared_ptr<Connection>> getIncoming();
        std::vector<std::shared_ptr<Connection>> getOutgoing();
};

} //neat_neat

#endif