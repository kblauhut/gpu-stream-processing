#include "physical_select.h"
#include <iostream>

PhysicalSelect::PhysicalSelect(PhysicalOperator *producer_operator,
                               const std::vector<size_t> &columns)
    : PhysicalOperator(producer_operator) {
  this->columns = columns;
}

void PhysicalSelect::processTuple(Tuple *input_tuple) {
  Tuple output_tuple = Tuple();

  publishTuple(output_tuple);

  counter++;
  if (counter % 10000 == 0) {
    std::cout << "Processed " << counter << " tuples" << std::endl;
  }
}
