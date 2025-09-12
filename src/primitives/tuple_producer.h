#pragma once

#include "../primitives/tuple.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

class TupleConsumer;

class TupleProducer {
public:
  TupleSchema output_schema;

protected:
  std::unordered_map<TupleConsumer *, size_t> consumer_indices;
  std::vector<Tuple *> consumer_backlog;
  size_t current_tuple_index = 0;

public:
  TupleProducer(TupleSchema output_schema)
      : output_schema(output_schema), consumer_indices(), consumer_backlog() {}
  ~TupleProducer() = default;

  void publishTuple(Tuple *tuple);

  virtual void run() = 0;

  void registerConsumer(TupleConsumer *consumer);
  Tuple *getCurrentTuple(TupleConsumer *consumer);
  void ackCurrentTuple(TupleConsumer *consumer);
};
