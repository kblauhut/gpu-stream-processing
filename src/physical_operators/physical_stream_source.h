#pragma once

#include "../primitives/tuple_schema.h"
#include "physical_operator.h"
#include <cstddef>
#include <sstream>
#include <string>

class PhysicalStreamSource : public PhysicalOperator {
  int stream_id;

public:
  explicit PhysicalStreamSource(PhysicalOperator *producer_operator,
                                int stream_id, TupleSchema output_schema);
  void processTuple(Tuple *input_tuple);
};
