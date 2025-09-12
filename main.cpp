#include "src/physical_operators/physical_predicate.h"
#include "src/physical_operators/physical_project.h"
#include "src/primitives/runnable.h"
#include "src/primitives/tuple_schema.h"
#include "src/sink/physical_sink.h"
#include "src/stream/physical_file_stream.h"
#include "src/stream/physical_stream.h"
#include <iostream>
#include <thread>

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

  auto t1 = std::thread([&physicalFileStream] {
    RunnableState state;
    do {
      state = physicalFileStream.run();
    } while (state != RunnableState::CLOSED);
  });

  auto t2 = std::thread([&physicalStream] {
    RunnableState state;
    do {
      state = physicalStream.run();
    } while (state != RunnableState::CLOSED);
  });

  auto t3 = std::thread([&physicalProjectOp] {
    RunnableState state;
    do {
      state = physicalProjectOp.run();
    } while (state != RunnableState::CLOSED);
  });

  auto t4 = std::thread([&physicalPredicateOp] {
    RunnableState state;
    do {
      state = physicalPredicateOp.run();
    } while (state != RunnableState::CLOSED);
  });

  auto t5 = std::thread([&physicalSink] {
    RunnableState state;
    do {
      state = physicalSink.run();
    } while (state != RunnableState::CLOSED);
  });

  std::cout << "Running threads..." << std::endl;
  t1.join();
  std::cout << "Thread 1 joined successfully." << std::endl;
  t2.join();
  std::cout << "Thread 2 joined successfully." << std::endl;
  t3.join();
  std::cout << "Thread 3 joined successfully." << std::endl;
  t4.join();
  std::cout << "Thread 4 joined successfully." << std::endl;
  t5.join();
  std::cout << "Thread 5 joined successfully." << std::endl;
  std::cout << "All threads joined successfully." << std::endl;

  // while (true) {
  //   physicalFileStream.run();
  //   physicalStream.run();
  //   physicalProjectOp.run();
  //   physicalPredicateOp.run();
  //   physicalSink.run();
  // }

  return 0;
}
