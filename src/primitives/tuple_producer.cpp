#include "tuple_producer.h"

Tuple *TupleProducer::getCurrentTuple(TupleConsumer *consumer) {
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
  consumer_indices[consumer]++;
}

void TupleProducer::publishTuple(Tuple *tuple) {
  consumer_backlog.push_back(tuple);
  this->current_tuple_index++;
}

bool TupleProducer::isClosed() const { return is_closed; }
