#include "physical_stream_source.h"

PhysicalStreamSource::PhysicalStreamSource(PhysicalOperator *producer_operator,
                                           int stream_id,
                                           TupleSchema output_schema)
    : PhysicalOperator(producer_operator, output_schema), stream_id(stream_id) {
}

void PhysicalStreamSource::processTuple(Tuple *input_tuple) {
  int stream_id = input_tuple->getInt(0); // Get stream id

  if (stream_id != this->stream_id) {
    return;
  }

  std::string stream_tuple_raw = input_tuple->getString(1);
  Tuple *output_tuple = new Tuple();

  std::istringstream iss(stream_tuple_raw);
  std::string token;
  int idx = 0;

  while (iss >> token) {
    DataType type = output_schema.getDataType(idx);

    switch (type) {
    case DataType::INTEGER: {
      size_t value = std::stoll(token);
      output_tuple->pushInt(value);
      break;
    }

    case DataType::STRING: {
      output_tuple->pushString(token);
      break;
    }
    }

    idx++;
  }

  publishTuple(output_tuple);
}
