# MPI_Info_set_json example

Shows how a CMake project uses the installed `mpi-extensions` library. The
program takes a key and a value from the command line, sets them on an
`MPI_Info` object via `MPI_Info_set_json`, and reads the value back.

## Build

This example requires an installed `mpi-extensions` library. See the
[examples README](../README.md) for how to install it and how to point CMake at
a custom install directory.

```sh
cmake -B build -DCMAKE_PREFIX_PATH=<install-dir>   # omit if installed to a default prefix
cmake --build build
```

## Run

```sh
mpirun -n 2 ./build/info_set_json_example cb_nodes 4
```

Output:

```
JSON input : {"cb_nodes": "4"}
info["cb_nodes"] = "4"
```

Keys and values are JSON-escaped, so they may contain quotes or backslashes.
