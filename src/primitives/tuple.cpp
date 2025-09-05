#include "tuple.h"

void Tuple::pushInt(size_t value) {
  unsigned int offset = data.size();
  data.push_back(value);
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
  return data[offsets[index]];
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
