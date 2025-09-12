#include "tuple_schema.h"

TupleSchema::TupleSchema(std::vector<DataType> dataTypes)
    : dataTypes(dataTypes) {}

DataType TupleSchema::getDataType(size_t index) { return dataTypes[index]; }

void TupleSchema::print() {
  std::cout << "Tuple Schema: ";
  for (size_t i = 0; i < dataTypes.size(); ++i) {
    DataType dataType = dataTypes[i];
    std::string dataTypeName =
        dataType == DataType::INTEGER ? "INTEGER" : "STRING";
    std::cout << dataTypeName;
    if (i < dataTypes.size() - 1)
      std::cout << ", ";
  }
  std::cout << std::endl;
}
