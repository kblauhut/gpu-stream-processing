#include "physical_sink.h"

void PhysicalSink::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    return;
  }

  consumeTuple(input_tuple);
  this->producers[0]->ackCurrentTuple(this);
}

void PhysicalSink::consumeTuple(Tuple *tuple) {
  counter++;
  if (counter % 1000 == 0) {
    std::cout << "Processed " << counter << " tuples" << std::endl;
  }
}
