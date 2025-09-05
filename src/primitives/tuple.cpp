#include "tuple.h"
#include "tuple_schema.h"
#include <iostream>

void Tuple::pushInt(size_t value) {
  unsigned int offset = data.size();
  const char *raw = reinterpret_cast<const char *>(&value);
  data.insert(data.end(), raw, raw + sizeof(size_t));
  offsets.push_back(offset);
}

void Tuple::pushString(const std::string &value) {
  unsigned int offset = data.size();
  data.insert(data.end(), value.begin(), value.end());
  offsets.push_back(offset);
}

int Tuple::getInt(size_t index) const {
  if (index >= offsets.size()) {
    throw std::out_of_range("Index out of range");
  }

  size_t value;
  std::memcpy(&value, data.data() + offsets[index], sizeof(size_t));
  return value;
}

std::string Tuple::getString(size_t index) const {
  if (index >= offsets.size()) {
    throw std::out_of_range("Index out of range");
  }
  unsigned int start = offsets[index];
  unsigned int end =
      index + 1 < offsets.size() ? offsets[index + 1] : data.size();
  return std::string(data.begin() + start, data.begin() + end);
}

void Tuple::printWithSchema(TupleSchema schema) const {

  for (size_t i = 0; i < offsets.size(); ++i) {
    if (schema.getDataType(i) == DataType::INTEGER) {
      std::cout << getInt(i) << " ";
    } else if (schema.getDataType(i) == DataType::STRING) {
      std::cout << getString(i) << " ";
    }
  }
  std::cout << std::endl;
}
