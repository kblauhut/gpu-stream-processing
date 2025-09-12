#include "physical_project.h"

void PhysicalProject::consumeTuple(Tuple *input_tuple) {
  auto &producer_schema = producers[0]->output_schema;

  Tuple *output_tuple = new Tuple();
  auto column_count = options.columns.size();

  for (size_t i = 0; i < column_count; i++) {
    DataType data_type = producer_schema.getDataType(options.columns[i]);

    switch (data_type) {
    case DataType::INTEGER: {
      size_t value = input_tuple->getInt(options.columns[i]);
      output_tuple->pushInt(value);
      break;
    }

    case DataType::STRING: {
      std::string token = input_tuple->getString(options.columns[i]);
      output_tuple->pushString(token);
      break;
    }
    }
  }

  publishTuple(output_tuple);
}

TupleSchema PhysicalProject::generateOutputSchema(
    std::vector<TupleProducer *> producers,
    PhysicalProjectOptions *operator_options) {
  auto &columns = operator_options->columns;
  auto column_count = columns.size();
  auto &producer_schema = producers[0]->output_schema;

  std::vector<DataType> dataTypes(column_count);

  for (size_t i = 0; i < column_count; i++) {
    DataType data_type = producer_schema.getDataType(columns[i]);
    dataTypes[i] = data_type;
  }

  return TupleSchema(dataTypes);
}
