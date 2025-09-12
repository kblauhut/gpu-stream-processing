#pragma once

#include "physical_operator.h"
#include <cstddef>

class PhysicalProject : public PhysicalOperator {
private:
  std::vector<size_t> columns;
  TupleSchema producer_schema;

public:
  PhysicalProject(std::vector<TupleProducer *> producers,
                  TupleSchema output_schema, const std::vector<size_t> &columns,
                  TupleSchema producer_schema)
      : PhysicalOperator(producers, output_schema),
        producer_schema(producer_schema) {
    this->columns = columns;
  }

  void consumeTuple(Tuple *input_tuple);
};
