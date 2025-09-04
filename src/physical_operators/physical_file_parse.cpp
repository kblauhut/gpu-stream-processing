#include "physical_file_parse.h"

PhysicalFileParse::PhysicalFileParse(std::string file_path)
    : PhysicalOperator() {
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

void PhysicalFileParse::produce() {
  if (file_buffer_offset >= file_buffer.size()) {
    return;
  }

  size_t line_end = file_buffer_offset;
  while (line_end < file_buffer.size() && file_buffer[line_end] != '\n') {
    ++line_end;
  }

  // Extract the line as a string (optional)
  std::string line(file_buffer.begin() + file_buffer_offset,
                   file_buffer.begin() + line_end);

  // Move offset past the newline character for the next call
  file_buffer_offset =
      (line_end < file_buffer.size()) ? line_end + 1 : line_end;
}
