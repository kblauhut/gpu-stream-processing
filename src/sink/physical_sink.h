#pragma once

#include "../primitives/tuple_consumer.h"
#include <iostream>

class PhysicalSink : public TupleConsumer {
private:
  size_t counter = 0;

public:
  PhysicalSink(std::vector<TupleProducer *> producers = {})
      : TupleConsumer(producers) {}

  void consumeTuple(Tuple *tuple);
  void run();
};
