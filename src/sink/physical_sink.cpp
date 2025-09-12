#include "physical_sink.h"

RunnableState PhysicalSink::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    if (this->producers[0]->isClosed()) {
      return RunnableState::CLOSED;
    }
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
