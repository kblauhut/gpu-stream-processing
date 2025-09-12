#include "src/physical_operators/physical_predicate.h"
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

  auto physicalStream =
      PhysicalStream(&physicalFileStream, {
                                              .output_schema = schema,
                                              .stream_id = 0,
                                          });

  auto physicalProjectOp =
      PhysicalProject({&physicalStream}, {
                                             .columns = {1, 2, 3},
                                         });

  auto physicalPredicateOp = PhysicalPredicate(
      {&physicalProjectOp}, {
                                .comparison_op = ComparisonOp::EQUAL,
                                .l_value_column = 0,
                                .r_value = 1011,
                                .r_source = ValueSource::LITERAL,
                            });

  auto physicalSink = PhysicalSink({&physicalPredicateOp});

  while (true) {
    physicalFileStream.run();
    physicalStream.run();
    physicalProjectOp.run();
    physicalPredicateOp.run();
    physicalSink.run();
  }

  return 0;
}
