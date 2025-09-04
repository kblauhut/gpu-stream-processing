#pragma once

#include "physical_operator.h"

class PhysicalSelect : public PhysicalOperator {
private:
  std::vector<size_t> columns;

public:
  PhysicalSelect(PhysicalOperator *producer_operator,
                 const std::vector<size_t> &columns);
  void produce();
};
