#include "physical_predicate.h"

void PhysicalPredicate::consumeTuple(Tuple *input_tuple) {
  bool predicate_result = predicate_function(input_tuple);

  if (predicate_result) {
    Tuple *output_tuple = new Tuple(*input_tuple);
    publishTuple(output_tuple);
  }
}

TupleSchema PhysicalPredicate::generateOutputSchema(
    std::vector<TupleProducer *> producers,
    PhysicalPredicateOptions *operator_options) {
  auto producer_schema = producers[0]->output_schema;
  return producer_schema;
}

std::function<bool(Tuple *)> PhysicalPredicate::buildPredicateFunction(
    std::vector<TupleProducer *> producers,
    PhysicalPredicateOptions *operator_options) {
  auto &producer_schema = producers[0]->output_schema;
  DataType l_value_type =
      producer_schema.getDataType(operator_options->l_value_column);
  DataType r_value_type;

  switch (operator_options->r_source) {
  case ValueSource::LITERAL:
    if (std::holds_alternative<int>(operator_options->r_value)) {
      r_value_type = DataType::INTEGER;
    }
    if (std::holds_alternative<std::string>(operator_options->r_value)) {
      r_value_type = DataType::STRING;
    }
    break;
  case ValueSource::COLUMN:
    r_value_type =
        producer_schema.getDataType(std::get<int>(operator_options->r_value));
    break;
  }

  if (l_value_type != r_value_type) {
    throw std::runtime_error("Comparison type mismatch in predicate");
  }

  std::function<bool(Tuple *)> predicate_function;
  switch (l_value_type) {
  case DataType::INTEGER: {
    std::function<int(Tuple *)> int_lhs_getter;
    std::function<int(Tuple *)> int_rhs_getter;

    int lcol = operator_options->l_value_column;
    int_lhs_getter = [lcol](Tuple *t) { return t->getInt(lcol); };

    switch (operator_options->r_source) {
    case ValueSource::LITERAL: {
      int literal = std::get<int>(operator_options->r_value);
      int_rhs_getter = [literal](Tuple *) { return literal; };
      break;
    }
    case ValueSource::COLUMN: {
      int rcol = std::get<int>(operator_options->r_value);
      int_rhs_getter = [rcol](Tuple *t) { return t->getInt(rcol); };
      break;
    }
    }

    switch (operator_options->comparison_op) {
    case ComparisonOp::EQUAL:
      return [int_lhs_getter, int_rhs_getter](Tuple *t) {
        return int_lhs_getter(t) == int_rhs_getter(t);
      };
    case ComparisonOp::NOT_EQUAL:
      return [int_lhs_getter, int_rhs_getter](Tuple *t) {
        return int_lhs_getter(t) != int_rhs_getter(t);
      };
    case ComparisonOp::LESS_THAN:
      return [int_lhs_getter, int_rhs_getter](Tuple *t) {
        return int_lhs_getter(t) < int_rhs_getter(t);
      };
    case ComparisonOp::LESS_THAN_OR_EQUAL:
      return [int_lhs_getter, int_rhs_getter](Tuple *t) {
        return int_lhs_getter(t) <= int_rhs_getter(t);
      };
    case ComparisonOp::GREATER_THAN:
      return [int_lhs_getter, int_rhs_getter](Tuple *t) {
        return int_lhs_getter(t) > int_rhs_getter(t);
      };
    case ComparisonOp::GREATER_THAN_OR_EQUAL:
      return [int_lhs_getter, int_rhs_getter](Tuple *t) {
        return int_lhs_getter(t) >= int_rhs_getter(t);
      };
    default:
      throw std::runtime_error("Unsupported comparison operator");
    }
  }
  case DataType::STRING: {
    std::function<std::string(Tuple *)> string_lhs_getter;
    std::function<std::string(Tuple *)> string_rhs_getter;

    int lcol = operator_options->l_value_column;
    string_lhs_getter = [lcol](Tuple *t) { return t->getString(lcol); };

    switch (operator_options->r_source) {
    case ValueSource::LITERAL: {
      std::string literal = std::get<std::string>(operator_options->r_value);
      string_rhs_getter = [literal](Tuple *) { return literal; };
      break;
    }
    case ValueSource::COLUMN: {
      int rcol = std::get<int>(operator_options->r_value);
      string_rhs_getter = [rcol](Tuple *t) { return t->getString(rcol); };
      break;
    }
    }

    switch (operator_options->comparison_op) {
    case ComparisonOp::EQUAL:
      return [string_lhs_getter, string_rhs_getter](Tuple *t) {
        return string_lhs_getter(t) == string_rhs_getter(t);
      };
    case ComparisonOp::NOT_EQUAL:
      return [string_lhs_getter, string_rhs_getter](Tuple *t) {
        return string_lhs_getter(t) != string_rhs_getter(t);
      };
    default:
      throw std::runtime_error("Unsupported comparison operator");
    }
  }
  }
};
