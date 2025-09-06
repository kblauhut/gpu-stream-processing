#pragma once

#include "physical_operator.h"
#include <charconv>
#include <fstream>
#include <stdexcept>

class PhysicalFileParse : public PhysicalOperator {
private:
  std::vector<char> file_buffer;
  size_t file_buffer_offset = 0;

public:
  explicit PhysicalFileParse(TupleSchema output_schema, std::string file_path);

  void processTuple(Tuple *input_tuple) override { return; };
  void run() override {
    if (file_buffer_offset >= file_buffer.size()) {
      return;
    }

    size_t first_token_end = file_buffer_offset;
    while (first_token_end < file_buffer.size() &&
           file_buffer[first_token_end] != ' ') {
      first_token_end++;
    }

    size_t line_end = file_buffer_offset;
    while (line_end < file_buffer.size() && file_buffer[line_end] != '\n') {
      line_end++;
    }

    int stream_type = 0;
    std::from_chars(file_buffer.data() + file_buffer_offset,
                    file_buffer.data() + first_token_end, stream_type);

    std::string raw_line_data(file_buffer.begin() + first_token_end + 1,
                              file_buffer.begin() + line_end);

    file_buffer_offset =
        (line_end < file_buffer.size()) ? line_end + 1 : line_end;

    Tuple tuple = Tuple();

    tuple.pushInt(stream_type);
    tuple.pushString(raw_line_data);

    publishTuple(tuple);
  }
};
