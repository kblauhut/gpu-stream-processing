#pragma once

#include <vector>

class Tuple {
private:
  std::vector<char> data;
  std::vector<unsigned int> offsets;

public:
  explicit Tuple(const std::string &raw_data);

  int getInt(size_t index) const;
  std::string getString(size_t index) const;
};
