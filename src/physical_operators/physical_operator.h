#pragma once

#include "../primitives/tuple.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

class PhysicalOperator {
private:
  std::unordered_map<PhysicalOperator *, size_t> consumer_indices;
  std::vector<Tuple> consumer_backlog;
  size_t current_tuple_index = 0;

protected:
  PhysicalOperator *producer_operator = nullptr;
  void publishTuple(const Tuple &tuple);

public:
  PhysicalOperator();
  explicit PhysicalOperator(PhysicalOperator *producer_operator);
  virtual ~PhysicalOperator() = default;

  virtual void produce() = 0;

  Tuple *consume(PhysicalOperator *consumer);
  void registerConsumerOperator(PhysicalOperator *consumer_operator);
  void onTupleConsumed(PhysicalOperator *consumer);
};
