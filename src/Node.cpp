#include <memory>

#include "../neat-neat/Node.hpp"

namespace neat_neat {

Node::Node(NodeType type)
    : type(type),
      value(0.0) {
}

NodeType Node::getType() {
    return this->type;
}

double Node::getValue() {
    return this->value;
}

void Node::setValue(double value) {
    this->value = value;
}

std::vector<std::shared_ptr<Connection>>& Node::getIncoming() {
    return this->incoming;
}

std::vector<std::shared_ptr<Connection>>& Node::getOutgoing() {
    return this->outgoing;
}

} //neat_neat