# MPI extensions

Collection of extensions to the [Message Passing Interface (MPI) standard](https://www.mpi-forum.org/).

The following extensions to the MPI standard are provided in this collection:

| Function | Description |
| -------- | ----------- |
| MPI_Dims_weighted_create | Replacement of MPI_Dims_create that factorizes a number taking into account weights for the individual dimensions. This is useful for creating an optimized Cartesian process topology. |
| MPI_Info_set_json | Convenience function that sets (key, value) pairs to an MPI info object from a JSON string |

## Getting Started

### Prerequisites

The basic C version of the MPI extensions requires
* CMake 3.28 or newer
* C99 compatible compiler
* MPI-3.1 compliant MPI library
* json-c library

For the tests:
* C++14 compatible compiler
* [Catch2](https://github.com/catchorg/Catch2) v3 test framework

### Installing

To build, test and install the mpi-extensions library execute
```shell
cmake -B build -S .
cmake --build build/
ctest --test-dir build/
cmake --install build/  --preffix <install-dir>
```

Tests are built by default. To build only the library, e.g. if Catch2 is not
available, disable them with `-DBUILD_TESTING=OFF`:
```shell
cmake -B build -S . -DBUILD_TESTING=OFF
```

If Catch2 or json-c are installed in a non-standard location, add their
installation directories to `CMAKE_PREFIX_PATH`.

## Contact

Christoph Niethammer <niethammer@hlrs.de>

