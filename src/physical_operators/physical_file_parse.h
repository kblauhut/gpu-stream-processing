#pragma once

#include "physical_operator.h"
#include <fstream>
#include <stdexcept>

class PhysicalFileParse : public PhysicalOperator {
private:
  std::vector<char> file_buffer;
  size_t file_buffer_offset = 0;

public:
  explicit PhysicalFileParse(std::string file_path);

  void produce();
};
