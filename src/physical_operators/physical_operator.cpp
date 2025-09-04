#include "../primitives/tuple.cpp"
#include <cstddef>
#include <unordered_map>
#include <vector>

class PhysicalOperator {
private:
  std::unordered_map<PhysicalOperator *, size_t> consumer_indices;
  std::vector<Tuple> consumer_backlog;

  size_t current_tuple_index = 0;

protected:
  PhysicalOperator *producer_operator;
  void publishTuple(const Tuple &tuple) {
    consumer_backlog.push_back(std::move(tuple));
    current_tuple_index++;
  }

public:
  PhysicalOperator(PhysicalOperator *producer_operator) {
    this->producer_operator = producer_operator;
    this->producer_operator->registerConsumerOperator(this);
  }

  void registerConsumerOperator(PhysicalOperator *consumer_operator) {
    consumer_indices[consumer_operator] = current_tuple_index;
  }

  Tuple *next(PhysicalOperator *consumer) {
    if (current_tuple_index == 0) {
      return nullptr;
    }

    size_t consumer_backlog_size = consumer_backlog.size();
    size_t consumer_index = consumer_indices[consumer];
    size_t index_at_backlog_start = current_tuple_index - consumer_backlog_size;
    size_t tuple_index = consumer_index - index_at_backlog_start;

    if (tuple_index > consumer_backlog_size) {
      return nullptr;
    }

    return &consumer_backlog[tuple_index];
  }
};
