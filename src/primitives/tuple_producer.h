#pragma once

#include "../primitives/tuple.h"
#include "runnable.h"
#include <cstddef>
#include <mutex>
#include <unordered_map>
#include <vector>

class TupleConsumer;

class TupleProducer {
public:
  TupleSchema output_schema;
  bool is_closed = false;
  std::mutex mutex;

protected:
  std::unordered_map<TupleConsumer *, size_t> consumer_indices;
  std::vector<Tuple *> consumer_backlog;
  size_t current_tuple_index = 0;

public:
  TupleProducer(TupleSchema output_schema)
      : output_schema(output_schema), consumer_indices(), consumer_backlog() {}
  ~TupleProducer() = default;

  virtual RunnableState run() = 0;

  void publishTuple(Tuple *tuple);
  void registerConsumer(TupleConsumer *consumer);
  Tuple *getCurrentTuple(TupleConsumer *consumer);
  void ackCurrentTuple(TupleConsumer *consumer);
  bool isClosed() const;

  // Metrics
  int getUnprocessedTupleCount();
  int getCurrentTupleIndex() const;
};
