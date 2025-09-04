#include <iostream>

int main() {
  std::cout << "Hello, world!" << 3 << std::endl;

  return 0;
}

// class StreamSource -- For each distinct stream
// class PhysicalOperator -- Interface for physical operators
// class Sink -- Output sink for a query
// class Tuple -- Represents a tuple of values

/*
 * Operators should run concurrently instead of vulcano iterator
 */
