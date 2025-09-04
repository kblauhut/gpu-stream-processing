#pragma once

#include <vector>

class Tuple {
private:
  std::vector<char> data;
  std::vector<unsigned int> offsets;

public:
  void pushInt(int value);
  void pushString(const std::string &value);

  int getInt(size_t index) const;
  std::string getString(size_t index) const;
};
