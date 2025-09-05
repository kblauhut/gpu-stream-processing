#include "physical_operator.h"

PhysicalOperator::PhysicalOperator() = default;

PhysicalOperator::PhysicalOperator(PhysicalOperator *producer_operator) {
  this->producer_operator = producer_operator;
  this->producer_operator->registerConsumerOperator(this);
}

void PhysicalOperator::publishTuple(const Tuple &tuple) {
  consumer_backlog.push_back(tuple);
  this->current_tuple_index++;
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

  if (tuple_index > consumer_backlog_size) {
    return nullptr;
  }

  return &consumer_backlog[tuple_index];
}

void PhysicalOperator::registerConsumerOperator(
    PhysicalOperator *consumer_operator) {
  consumer_indices[consumer_operator] = this->current_tuple_index;
}

void PhysicalOperator::onTupleConsumed(PhysicalOperator *consumer) {
  consumer_indices[consumer]++;
}
