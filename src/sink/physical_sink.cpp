#include "physical_sink.h"
#include <thread>

RunnableState PhysicalSink::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    if (this->producers[0]->isClosed()) {
      return RunnableState::CLOSED;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return RunnableState::OPEN;
  }

  consumeTuple(input_tuple);
  this->producers[0]->ackCurrentTuple(this);

  return RunnableState::OPEN;
}

void PhysicalSink::consumeTuple(Tuple *tuple) {
  auto &producer_schema = producers[0]->output_schema;

  counter++;
  if (counter % 1 == 0) {
    tuple->printWithSchema(producer_schema);
    std::cout << "Processed " << counter << " tuples" << std::endl;
  }
}
