#include "physical_select.h"
#include <iostream>

PhysicalSelect::PhysicalSelect(PhysicalOperator *producer_operator,
                               const std::vector<size_t> &columns)
    : PhysicalOperator(producer_operator) {
  this->columns = columns;
}

void PhysicalSelect::produce() {
  Tuple *input_tuple = producer_operator->consume(this);
  if (!input_tuple) {
    return;
  }

  Tuple output_tuple = Tuple(*input_tuple);

  publishTuple(output_tuple);
  producer_operator->onTupleConsumed(this);

  counter++;
  if (counter % 10 == 0) {
    std::cout << "Processed " << counter << " tuples" << std::endl;
  }
}
