#include "physical_stream_source.h"
#include "physical_operator.h"

PhysicalStreamSource::PhysicalStreamSource(PhysicalOperator *producer_operator)
    : PhysicalOperator(producer_operator) {}

void PhysicalStreamSource::produce() {
  Tuple *input_tuple = producer_operator->consume(this);
  Tuple output_tuple = Tuple(*input_tuple);

  publishTuple(output_tuple);
}
