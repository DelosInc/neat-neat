#include "../include/neat-neat/Connection.hpp"

namespace neat_neat {

Connection::Connection(std::shared_ptr<Node>& in, std::shared_ptr<Node>& out, double weight,
                       bool enabled, size_t innovationNumber) 
    : in(in),
      out(out),
      weight(weight),
      enabled(enabled),
      innovationNumber(innovationNumber) {

}                

std::shared_ptr<Node>& Connection::getIn() {
    return this->in;
}

std::shared_ptr<Node>& Connection::getOut() {
  return this->out;
}

double Connection::getWeight() {
  return this->weight;
}

bool Connection::isEnabled() {
  return this->enabled;
}

size_t Connection::getInnovationNumber() {
  return this->innovationNumber;
}

void Connection::setWeight(double weight) {
  this->weight = weight;
}

void Connection::enable() {
  this->enabled = true;
}

void Connection::disable() {
  this->enabled = false;
}

} //neat_neat