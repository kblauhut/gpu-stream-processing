#pragma once

#include "../primitives/tuple.h"
#include "../primitives/tuple_consumer.h"
#include "../primitives/tuple_producer.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

class PhysicalOperator : public TupleProducer, public TupleConsumer {
public:
  PhysicalOperator(std::vector<TupleProducer *> producers,
                   TupleSchema output_schema)
      : TupleProducer(output_schema), TupleConsumer(producers) {
    for (auto &producer : producers) {
      producer->registerConsumer(this);
    }
  }

  virtual ~PhysicalOperator() = default;

  void run();
};
