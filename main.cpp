#include "src/physical_operators/physical_file_parse.h"
#include "src/physical_operators/physical_select.h"
#include "src/physical_operators/physical_stream_source.h"
#include "src/primitives/tuple_schema.h"

int main() {

  auto fileParseOp =
      PhysicalFileParse(TupleSchema({DataType::INTEGER, DataType::STRING}),
                        "../data/stream_data");

  auto streamSourceOp = PhysicalStreamSource(
      &fileParseOp, 0,
      TupleSchema({DataType::INTEGER, DataType::INTEGER, DataType::INTEGER,
                   DataType::STRING, DataType::STRING, DataType::INTEGER,
                   DataType::STRING}));

  auto selectOp = PhysicalSelect(
      &streamSourceOp,
      TupleSchema({DataType::INTEGER, DataType::INTEGER, DataType::STRING}),
      {1, 2, 3});

  while (true) {
    fileParseOp.run();
    streamSourceOp.run();
    selectOp.run();
  }

  return 0;
}
