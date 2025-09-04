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
  DataType getDataType(size_t index) { return dataTypes[index]; }
};
