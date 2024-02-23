# SplitL1Cache Simulator

## Overview
The SplitL1Cache Simulator is a C++ application designed to simulate the behavior of a split Level 1 (L1) cache in a 32-bit processor. It supports various operations such as read, write, fetch instruction, invalidate, and snoop, providing detailed statistics about cache hits, misses, and overall performance.

## Features
- Simulation of L1 data and instruction caches with configurable associativity and set sizes.
- Support for multiple cache operations including read, write, fetch instruction, invalidate, and snoop.
- Detailed statistics reporting including cache hits, misses, and hit ratios.
- Mode selection for varying levels of output detail.

## Getting Started

### Prerequisites
- GNU Compiler Collection (GCC) or any compatible C++ compiler supporting C++20.
- Make utility for building the project.

### Building the Project
Navigate to the project directory and use the `Makefile` to build the project:

```bash
cd splitL1cache
make
```

This will compile the source files and generate an executable named `splitl1cache`.

### Running the Simulator
To run the simulator, use the following command:

```bash
./splitl1cache <mode> <trace_file>
```

- `<mode>`: Operation mode of the simulator. Use `0` for a summary of usage statistics and responses to 9s in the trace file, or `1` for detailed output including communication messages to the L2 cache.
- `<trace_file>`: Path to the trace file containing the cache operations to simulate.

Example:

```bash
./splitl1cache 1 tests/tracefile1.txt
```

## Trace File Format
Trace files should contain lines with operations and addresses in hexadecimal format. Supported operations are:

- `0`: Read
- `1`: Write
- `2`: Fetch instruction
- `3`: Invalidate
- `4`: Snoop
- `8`: Clear cache
- `9`: Print statistics

Example line in a trace file:

```
0 984DE132
```

This line represents a read operation (`0`) at the address `0x984DE132`.

## Cleaning Up
To clean the build artifacts, run:

```bash
make clean
```

## Contributing
Contributions to the SplitL1Cache Simulator are welcome. Please ensure to follow the project's coding standards and submit a pull request for review.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
