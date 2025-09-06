#include "physical_operator.h"

PhysicalOperator::PhysicalOperator(TupleSchema output_schema)
    : output_schema(output_schema) {}

PhysicalOperator::PhysicalOperator(PhysicalOperator *producer_operator,
                                   TupleSchema output_schema)
    : output_schema(output_schema) {
  this->producer_operator = producer_operator;
  this->producer_operator->registerConsumerOperator(this);
}

void PhysicalOperator::publishTuple(Tuple *tuple) {
  consumer_backlog.push_back(tuple);
  this->current_tuple_index++;
}

void PhysicalOperator::run() {
  Tuple *input_tuple = producer_operator->consume(this);
  if (!input_tuple) {
    return;
  }

  processTuple(input_tuple);
  producer_operator->onTupleConsumed(this);
}

Tuple *PhysicalOperator::consume(PhysicalOperator *consumer) {
  if (this->current_tuple_index == 0) {
    return nullptr;
  }

  size_t consumer_backlog_size = consumer_backlog.size();
  size_t consumer_index = consumer_indices[consumer];
  size_t index_at_backlog_start =
      this->current_tuple_index - consumer_backlog_size;
  size_t tuple_index = consumer_index - index_at_backlog_start;

  if (tuple_index > consumer_backlog_size - 1) {
    return nullptr;
  }

  return consumer_backlog[tuple_index];
}

void PhysicalOperator::registerConsumerOperator(
    PhysicalOperator *consumer_operator) {
  consumer_indices[consumer_operator] = this->current_tuple_index;
}

void PhysicalOperator::onTupleConsumed(PhysicalOperator *consumer) {
  consumer_indices[consumer]++;
}
