#include "physical_operator.h"
#include <thread>

RunnableState PhysicalOperator::run() {
  Tuple *input_tuple = this->producers[0]->getCurrentTuple(this);
  if (!input_tuple) {
    if (this->producers[0]->isClosed()) {
      is_closed = true;
      return RunnableState::CLOSED;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return RunnableState::OPEN;
  }

  consumeTuple(input_tuple);
  this->producers[0]->ackCurrentTuple(this);
  return RunnableState::OPEN;
}
