#pragma once

#include "../primitives/tuple_consumer.h"
#include "../primitives/tuple_producer.h"
#include "../primitives/tuple_schema.h"
#include "physical_file_stream.h"
#include <cstddef>
#include <sstream>
#include <string>

class PhysicalStream : public TupleProducer, public TupleConsumer {
private:
  int stream_id;
  TupleSchema output_schema;

public:
  PhysicalStream(PhysicalFileStream *file_stream, int stream_id,
                 TupleSchema output_schema)
      : TupleProducer(), TupleConsumer({file_stream}), stream_id(stream_id),
        output_schema(output_schema) {}
  ~PhysicalStream() = default;

  void consumeTuple(Tuple *input_tuple) override;
  void run() override;
};
