#include "tuple_producer.h"
#include <mutex>

Tuple *TupleProducer::getCurrentTuple(TupleConsumer *consumer) {
  std::lock_guard lock(mutex);
  if (this->current_tuple_index == 0) {
    return nullptr;
  }

  size_t consumer_backlog_size = consumer_backlog.size();
  size_t consumer_index = consumer_indices[consumer];
  size_t index_at_backlog_start =
      this->current_tuple_index - consumer_backlog_size;
  size_t tuple_index = consumer_index - index_at_backlog_start;

  if (tuple_index > consumer_backlog_size - 1) {
    return nullptr;
  }

  return consumer_backlog[tuple_index];
}

void TupleProducer::registerConsumer(TupleConsumer *consumer_operator) {
  consumer_indices[consumer_operator] = this->current_tuple_index;
}

void TupleProducer::ackCurrentTuple(TupleConsumer *consumer) {
  std::lock_guard lock(mutex);
  consumer_indices[consumer]++;
}

void TupleProducer::publishTuple(Tuple *tuple) {
  std::lock_guard lock(mutex);
  consumer_backlog.push_back(tuple);
  this->current_tuple_index++;
}

bool TupleProducer::isClosed() const { return is_closed; }

int TupleProducer::getUnprocessedTupleCount() {
  std::lock_guard lock(mutex);
  size_t max_consumer_index = 0;
  for (auto &pair : consumer_indices) {
    max_consumer_index = std::max(max_consumer_index, pair.second);
  }
  return this->current_tuple_index - max_consumer_index;
}

int TupleProducer::getCurrentTupleIndex() const {
  return this->current_tuple_index;
}
