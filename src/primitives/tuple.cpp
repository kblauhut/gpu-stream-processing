#include <vector>

class Tuple {
private:
  std::vector<char> data;
  std::vector<uint> offsets;

public:
  Tuple(std::string raw_data) {}
  void getInt(size_t index) const {}
  void getString(size_t index) const {}
};
