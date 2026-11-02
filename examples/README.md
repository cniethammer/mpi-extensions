# Examples

This directory contains examples showing the usage of the various extensions provided by mpi-extensions. Each subdirectory is a standalone CMake project that uses an **installed**
`mpi-extensions` library, the same way your own application would.

| Example | Description |
| ------- | ----------- |
| [info_set_json](info_set_json/) | Set an MPI info (key, value) pair from the command line with `MPI_Info_set_json` |

## Prerequisites: install mpi-extensions

The examples are not built as part of the library. Build and install
`mpi-extensions` first, from the repository root:

```sh
cmake -B build -S . -DBUILD_TESTING=OFF
cmake --build build
cmake --install build --prefix <install-dir>
```

Omit `--prefix` to install to the default system prefix (usually `/usr/local`).

## Building an example

If `mpi-extensions` is in a default system prefix, CMake finds it automatically.
From within an example directory:

```sh
cmake -B build
cmake --build build
```

If you installed it to a custom directory, tell CMake where to find it by
passing that install prefix in `CMAKE_PREFIX_PATH`:

```sh
cmake -B build -DCMAKE_PREFIX_PATH=<install-dir>
cmake --build build
```

Alternatively:

- set it in the environment: `export CMAKE_PREFIX_PATH=<install-dir>`, or
- point directly at the package config directory:
  `-Dmpi-extensions_DIR=<install-dir>/lib/cmake/mpi-extensions`
  (on some platforms `lib64` instead of `lib`).

## Using mpi-extensions in your own CMake project

```cmake
project(my_app LANGUAGES C CXX)

find_package(mpi-extensions 0.1 REQUIRED)
find_package(MPI REQUIRED COMPONENTS CXX)   # only needed for C++ code

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE
    mpi-extensions::mpi-extensions
    MPI::MPI_CXX                            # only needed for C++ code
)
```

Then include `<mpi-extensions.h>` (or an individual header such as
`<MPI_Info_set_json.h>`). The target provides the include directory and MPI.

The C language must be enabled in your project, even for pure C++ code, because
the library links against `MPI::MPI_C`, which CMake's FindMPI only provides when
C is enabled.

C++ code that includes `mpi.h` must additionally link `MPI::MPI_CXX`. Some MPI
implementations (e.g. Open MPI 4.x) pull in their C++ bindings from `mpi.h`,
which then fail to link with only `MPI::MPI_C`.
