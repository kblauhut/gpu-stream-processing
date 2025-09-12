#pragma once

#include "physical_operator.h"
#include <cstddef>

struct PhysicalPredicateOptions {
  std::vector<size_t> columns;
};

class PhysicalPredicate : public PhysicalOperator {
private:
  PhysicalPredicateOptions options;

public:
  PhysicalPredicate(std::vector<TupleProducer *> producers,
                    PhysicalPredicateOptions options)
      : PhysicalOperator(producers, generateOutputSchema(producers, &options)),
        options(options) {}

  static TupleSchema
  generateOutputSchema(std::vector<TupleProducer *> producers,
                       PhysicalPredicateOptions *operator_options);
  void consumeTuple(Tuple *input_tuple);
};
