#pragma once

#include "physical_operator.h"
#include <cstddef>

class PhysicalSelect : public PhysicalOperator {
private:
  std::vector<size_t> columns;
  size_t counter;
  TupleSchema producer_schema;

public:
  PhysicalSelect(PhysicalOperator *producer_operator, TupleSchema output_schema,
                 const std::vector<size_t> &columns);
  void processTuple(Tuple *input_tuple);
};
