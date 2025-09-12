#pragma once

#include <iostream>
#include <vector>

enum class DataType {
  INTEGER = 0,
  STRING = 2,
};

class TupleSchema {
private:
  std::vector<DataType> dataTypes;

public:
  TupleSchema(std::vector<DataType> dataTypes);

  DataType getDataType(size_t index);
  void print();
};
