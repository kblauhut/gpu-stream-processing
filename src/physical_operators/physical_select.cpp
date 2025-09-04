#include "physical_operator.cpp"
#include <algorithm>

class PhysicalSelect : public PhysicalOperator {
private:
  std::vector<size_t> columns;

public:
  PhysicalSelect(PhysicalOperator *producer_operator,
                 const std::vector<size_t> &columns)
      : PhysicalOperator(producer_operator) {
    this->columns = columns;
  }

  void execute() {
    Tuple *input_tuple = producer_operator->next(this);
    Tuple output_tuple = Tuple(*input_tuple);

    publishTuple(output_tuple);
  }
};
