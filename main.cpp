#include "src/physical_operators/physical_file_parse.h"
#include "src/physical_operators/physical_select.h"
#include "src/physical_operators/physical_stream_source.h"
#include "src/primitives/tuple_schema.h"

int main() {

  auto fileParseOp = PhysicalFileParse("../data/stream_data");

  std::vector<DataType> dataTypes = {
      DataType::INTEGER, DataType::INTEGER, DataType::INTEGER, DataType::STRING,
      DataType::STRING,  DataType::INTEGER, DataType::STRING};
  auto tuple_schema = TupleSchema(dataTypes);
  auto streamSourceOp = PhysicalStreamSource(&fileParseOp, 0, tuple_schema);

  auto selectOp = PhysicalSelect(&streamSourceOp, {1, 2});

  while (true) {
    fileParseOp.run();
    streamSourceOp.run();
    selectOp.run();
  }

  return 0;
}
