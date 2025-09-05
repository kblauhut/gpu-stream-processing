#pragma once

#include "physical_operator.h"
#include <cstddef>

class PhysicalSelect : public PhysicalOperator {
private:
  std::vector<size_t> columns;
  size_t counter;

public:
  PhysicalSelect(PhysicalOperator *producer_operator,
                 const std::vector<size_t> &columns);
  void processTuple(Tuple *input_tuple);
};
