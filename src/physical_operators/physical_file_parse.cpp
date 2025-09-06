#include "physical_file_parse.h"
#include <iostream>

PhysicalFileParse::PhysicalFileParse(TupleSchema output_schema,
                                     std::string file_path)
    : PhysicalOperator(output_schema) {
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);

  if (!file) {
    throw std::runtime_error("Failed to open file: " + file_path);
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  file_buffer.resize(size);

  if (!file.read(file_buffer.data(), size)) {
    throw std::runtime_error("Failed to read file: " + file_path);
  }
}
