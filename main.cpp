#include "src/physical_operators/physical_project.h"
#include "src/primitives/tuple_schema.h"
#include "src/sink/physical_sink.h"
#include "src/stream/physical_file_stream.h"
#include "src/stream/physical_stream.h"

int main() {
  auto physicalFileStream = PhysicalFileStream("../data/stream_data");

  auto schema =
      TupleSchema({DataType::INTEGER, DataType::INTEGER, DataType::INTEGER,
                   DataType::STRING, DataType::STRING, DataType::INTEGER,
                   DataType::STRING});

  auto physicalStream = PhysicalStream(&physicalFileStream, 0, schema);
  auto physicalProjectOp1 = PhysicalProject(
      {&physicalStream},
      TupleSchema({DataType::INTEGER, DataType::INTEGER, DataType::STRING}),
      {1, 2, 3}, schema);

  auto physicalProjectOp2 = PhysicalProject(
      {&physicalStream}, TupleSchema({DataType::STRING}), {3}, schema);

  auto physicalSink1 = PhysicalSink({&physicalProjectOp1});
  auto physicalSink2 = PhysicalSink({&physicalProjectOp2});

  while (true) {
    physicalFileStream.run();
    physicalStream.run();
    physicalProjectOp1.run();
    physicalProjectOp2.run();
    physicalSink1.run();
    physicalSink2.run();
  }
  return 0;
}
