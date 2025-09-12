#pragma once

#include "runnable.h"
#include "tuple_producer.h"
#include <vector>

class TupleConsumer {
protected:
  std::vector<TupleProducer *> producers;

public:
  explicit TupleConsumer(std::vector<TupleProducer *> producers)
      : producers(producers) {}
  ~TupleConsumer() = default;

  virtual void consumeTuple(Tuple *input_tuple) = 0;
  virtual RunnableState run() = 0;
};
