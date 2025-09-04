#include "src/physical_operators/physical_file_parse.h"
#include "src/physical_operators/physical_select.h"
#include "src/physical_operators/physical_stream_source.h"
#include <iostream>
#include <vector>

int main() {

  std::cout << "Hello, World!" << std::endl;
  auto fileParseOp = PhysicalFileParse("../data/stream_data");
  std::cout << "File parsed successfully." << std::endl;
  auto streamSourceOp = PhysicalStreamSource(&fileParseOp);
  std::cout << "Stream source initialized successfully." << std::endl;
  auto selectOp = PhysicalSelect(&streamSourceOp, {1, 2});
  std::cout << "Select operator initialized successfully." << std::endl;

  while (true) {
    std::cout << "Starting iteration..." << std::endl;
    fileParseOp.produce();
    std::cout << "File parsing completed." << std::endl;
    streamSourceOp.produce();
    std::cout << "Stream source produced data." << std::endl;
    selectOp.produce();
    std::cout << "Select operator processed data." << std::endl;
  }

  return 0;
}
