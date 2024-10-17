# MPI extensions

Collection of extensions to the [Message Passing Interface (MPI) standard](https://www.mpi-forum.org/).

The following extensions to the MPI standard are provided in this collection:

| Function | Description |
| -------- | ----------- |
| MPI_Dims_weighted_create | Replacement of MPI_Dims_create that factorizes a number taking into account weights for the individual dimensions. This is usefull for creating an optimized Cartesian process topology. |
| MPI_Info_set_json | Convinience funtion that sets (key, value) pairs to an MPI info object from a JSON string |

## Getting Started

### Prerequisites

The basic C version of the MPI extensions requires
* C99 compatible compiler
* MPI-3.1 compliant MPI library
* json-c library

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

