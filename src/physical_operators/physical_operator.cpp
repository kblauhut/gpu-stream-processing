#include "physical_operator.h"

void PhysicalOperator::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    return;
  }

  consumeTuple(input_tuple);
  this->producers[0]->ackCurrentTuple(this);
}
