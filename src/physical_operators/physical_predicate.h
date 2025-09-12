#pragma once

#include "physical_operator.h"
#include <functional>
#include <string>
#include <sys/__endian.h>
#include <variant>

enum ComparisonOp {
  EQUAL,
  GREATER_THAN,
  LESS_THAN,
  GREATER_THAN_OR_EQUAL,
  LESS_THAN_OR_EQUAL,
  NOT_EQUAL
};

enum ValueSource { LITERAL, COLUMN };

struct PhysicalPredicateOptions {
  ComparisonOp comparison_op;
  int l_value_column;
  std::variant<int, std::string> r_value;
  ValueSource r_source;
};

class PhysicalPredicate : public PhysicalOperator {
private:
  std::function<bool(Tuple *)> predicate_function;
  PhysicalPredicateOptions options;

public:
  PhysicalPredicate(std::vector<TupleProducer *> producers,
                    PhysicalPredicateOptions options)
      : PhysicalOperator(producers, generateOutputSchema(producers, &options)),
        predicate_function(buildPredicateFunction(producers, &options)),
        options(options) {}

  static TupleSchema
  generateOutputSchema(std::vector<TupleProducer *> producers,
                       PhysicalPredicateOptions *operator_options);
  static std::function<bool(Tuple *)>
  buildPredicateFunction(std::vector<TupleProducer *> producers,
                         PhysicalPredicateOptions *operator_options);
  void consumeTuple(Tuple *input_tuple);
};
