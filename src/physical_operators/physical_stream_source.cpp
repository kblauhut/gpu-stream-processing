#include "physical_stream_source.h"

PhysicalStreamSource::PhysicalStreamSource(PhysicalOperator *producer_operator,
                                           int stream_id,
                                           TupleSchema &tuple_schema)
    : PhysicalOperator(producer_operator), stream_id(stream_id),
      tuple_schema(tuple_schema) {}

void PhysicalStreamSource::produce() {
  Tuple *input_tuple = producer_operator->consume(this);
  if (!input_tuple) {
    return;
  }

  int stream_id = input_tuple->getInt(0); // Get stream id

  if (stream_id != this->stream_id) {
    producer_operator->onTupleConsumed(this);
    return;
  }

  std::string stream_tuple_raw = input_tuple->getString(1);
  Tuple output_tuple = Tuple();

  std::istringstream iss(stream_tuple_raw);
  std::string token;
  int idx = 0;

  while (iss >> token) {
    DataType type = tuple_schema.getDataType(idx);

    switch (type) {
    case DataType::INTEGER: {
      size_t value = std::stoll(token);
      output_tuple.pushInt(value);
      break;
    }

    case DataType::STRING: {
      output_tuple.pushString(token);
      break;
    }
    default: {
      throw std::runtime_error("Unsupported DataType at index " +
                               std::to_string(idx));
    }
    }

    idx++;
  }

  publishTuple(output_tuple);
  producer_operator->onTupleConsumed(this);
}
