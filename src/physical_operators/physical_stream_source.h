#pragma once

#include "physical_operator.h"

class PhysicalStreamSource : public PhysicalOperator {
public:
  explicit PhysicalStreamSource(PhysicalOperator *producer_operator);
  void produce();
};
