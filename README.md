# MPI extensions

Collection of extensions to the [Message Passing Interface (MPI) standard](https://www.mpi-forum.org/).

## Getting Started

### Prerequisites

The basic C version of the MPI extensions requires
* C99 compatible compiler
* MPI-3.1 compliant MPI library

For the tests:
* C++11 compatible compiler
* catch2 test framework

### Installing

To build, test and install the mpi-extensions library execute
```shell
mkdir build && cd build
cmake ..
make
make test
make install
```

## Contact

Christoph Niethammer <niethammer@hlrs.de>

