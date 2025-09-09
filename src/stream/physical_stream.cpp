#include "physical_stream.h"

void PhysicalStream::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    return;
  }

  consumeTuple(input_tuple);

  this->producers[0]->ackCurrentTuple(this);
}

void PhysicalStream::consumeTuple(Tuple *input_tuple) {
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
