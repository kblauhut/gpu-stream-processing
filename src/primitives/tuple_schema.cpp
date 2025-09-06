#include "tuple_schema.h"

TupleSchema::TupleSchema(std::vector<DataType> dataTypes)
    : dataTypes(dataTypes) {}

DataType TupleSchema::getDataType(size_t index) { return dataTypes[index]; }
