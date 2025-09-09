# gpu-stream-processing
This is a repo for my bachelor's thesis project comparing NVIDIA MPS to MIG in stream processing workloads.

Query Setup:
StreamSource -> PhysicalOperator -> PhysicalOperator -> PhysicalOperator -> Sink

StreamSource: Producer
PhysicalOperator: ProducerConsumer
PhysicalOperator: ProducerConsumer
PhysicalOperator: ProducerConsumer
Sink: Consumer

Producers have multiple outputs
Consumers have multiple inputs
