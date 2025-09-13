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

  auto t_metrics = std::thread([&physicalFileStream] {
    int last_tuple_index = 0;
    auto last_time_stamp = std::chrono::steady_clock::now();

    while (!physicalFileStream.isClosed()) {
      auto current_tuple_index = physicalFileStream.getCurrentTupleIndex();
      auto current_time_stamp = std::chrono::steady_clock::now();
      double period_duration_s =
          std::chrono::duration_cast<std::chrono::nanoseconds>(
              current_time_stamp - last_time_stamp)
              .count() /
          1e9;

      double tuples_per_second =
          (current_tuple_index - last_tuple_index) / period_duration_s;
      last_tuple_index = current_tuple_index;
      last_time_stamp = current_time_stamp;

      std::cout << "Tuples per second: " << tuples_per_second << std::endl;
      std::cout << "Unprocessed tuples: "
                << physicalFileStream.getUnprocessedTupleCount() << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
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

  t_metrics.join();
  std::cout << "Metrics thread joined successfully." << std::endl;

  std::cout << "All threads joined successfully." << std::endl;

  // while (true) {
  //   physicalFileStream.run();
  //   physicalStream.run();
  //   physicalProjectOp.run();
  //   physicalPredicateOp.run();
  //   auto complete = physicalSink.run();
  //   if (complete == RunnableState::CLOSED) {
  //     break;
  //   }
  // }

  return 0;
}
