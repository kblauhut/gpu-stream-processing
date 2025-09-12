#include "physical_file_stream.h"

PhysicalFileStream::PhysicalFileStream(std::string file_path)
    : TupleProducer(TupleSchema({DataType::INTEGER, DataType::STRING})) {
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

RunnableState PhysicalFileStream::run() {
  if (file_buffer_offset >= file_buffer.size()) {
    is_closed = true;
    return RunnableState::CLOSED;
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

  Tuple *tuple = new Tuple();

  tuple->pushInt(stream_type);
  tuple->pushString(raw_line_data);

  publishTuple(tuple);
  return RunnableState::OPEN;
}
