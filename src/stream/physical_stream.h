#pragma once

#include "../primitives/tuple_consumer.h"
#include "../primitives/tuple_producer.h"
#include "../primitives/tuple_schema.h"
#include "physical_file_stream.h"
#include <cstddef>
#include <sstream>
#include <string>

struct PhysicalStreamOptions {
  int stream_id;
  TupleSchema output_schema;
};

class PhysicalStream : public TupleProducer, public TupleConsumer {
private:
  PhysicalStreamOptions options;

public:
  PhysicalStream(PhysicalFileStream *file_stream, PhysicalStreamOptions options)
      : TupleProducer(options.output_schema), TupleConsumer({file_stream}),
        options(options) {}
  ~PhysicalStream() = default;

  void consumeTuple(Tuple *input_tuple) override;
  RunnableState run() override;
};
