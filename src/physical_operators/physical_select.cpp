#include "physical_select.h"

PhysicalSelect::PhysicalSelect(PhysicalOperator *producer_operator,
                               const std::vector<size_t> &columns)
    : PhysicalOperator(producer_operator) {
  this->columns = columns;
}

void PhysicalSelect::produce() {
  Tuple *input_tuple = producer_operator->consume(this);
  Tuple output_tuple = Tuple(*input_tuple);

  publishTuple(output_tuple);
}
