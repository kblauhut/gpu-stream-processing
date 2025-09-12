#pragma once

#include "physical_operator.h"
#include <cstddef>

struct PhysicalProjectOptions {
  std::vector<size_t> columns;
};

class PhysicalProject : public PhysicalOperator {
private:
  PhysicalProjectOptions options;

public:
  PhysicalProject(std::vector<TupleProducer *> producers,
                  PhysicalProjectOptions options)
      : PhysicalOperator(producers, generateOutputSchema(producers, &options)),
        options(options) {}

  static TupleSchema
  generateOutputSchema(std::vector<TupleProducer *> producers,
                       PhysicalProjectOptions *operator_options);
  void consumeTuple(Tuple *input_tuple);
};
