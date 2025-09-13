#include "physical_stream.h"
#include <thread>

RunnableState PhysicalStream::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    if (this->producers[0]->isClosed()) {
      is_closed = true;
      return RunnableState::CLOSED;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return RunnableState::OPEN;
  }

  consumeTuple(input_tuple);
  this->producers[0]->ackCurrentTuple(this);

  return RunnableState::OPEN;
}

void PhysicalStream::consumeTuple(Tuple *input_tuple) {
  int stream_id = input_tuple->getInt(0); // Get stream id

  if (stream_id != options.stream_id) {
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
