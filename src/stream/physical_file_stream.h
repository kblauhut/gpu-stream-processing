#pragma once

#include "../primitives/tuple_producer.h"
#include <iostream>

#include <charconv>
#include <fstream>
#include <stdexcept>

class PhysicalFileStream : public TupleProducer {
private:
  std::vector<char> file_buffer;
  size_t file_buffer_offset = 0;

public:
  PhysicalFileStream(std::string file_path);
  ~PhysicalFileStream() = default;

  RunnableState run() override;
};
