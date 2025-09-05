#pragma once

#include <vector>

enum class DataType {
  INTEGER = 0,
  FLOAT = 1,
  STRING = 2,
};

class TupleSchema {
private:
  std::vector<DataType> dataTypes;

public:
  TupleSchema(std::vector<DataType> &dataTypes);

  DataType getDataType(size_t index);
};
