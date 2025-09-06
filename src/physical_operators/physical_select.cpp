#include "physical_select.h"
#include <iostream>

PhysicalSelect::PhysicalSelect(PhysicalOperator *producer_operator,
                               TupleSchema output_schema,
                               const std::vector<size_t> &columns)
    : PhysicalOperator(producer_operator, output_schema),
      producer_schema(producer_operator->output_schema) {
  this->columns = columns;
}

void PhysicalSelect::processTuple(Tuple *input_tuple) {
  Tuple *output_tuple = new Tuple();

  for (size_t i = 0; i < columns.size(); i++) {
    DataType data_type = producer_schema.getDataType(columns[i]);

    switch (data_type) {
    case DataType::INTEGER: {
      size_t value = input_tuple->getInt(columns[i]);
      output_tuple->pushInt(value);
      break;
    }

    case DataType::STRING: {
      std::string token = input_tuple->getString(columns[i]);
      output_tuple->pushString(token);
      break;
    }
    }
  }

  publishTuple(output_tuple);

  counter++;
  if (counter % 10000 == 0) {
    std::cout << "Processed " << counter << " tuples" << std::endl;
  }
}
