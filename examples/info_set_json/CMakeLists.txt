cmake_minimum_required(VERSION 3.28)
# C must be enabled as well: the library links MPI::MPI_C, which FindMPI only
# provides when the C language is enabled.
project(info_set_json_example LANGUAGES C CXX)

# Locate the installed library, e.g. configure with
#   cmake -B build -DCMAKE_PREFIX_PATH=/path/to/mpi-extensions/install
find_package(mpi-extensions 0.1 REQUIRED)

# C++ code that includes mpi.h must also link MPI::MPI_CXX: some MPI
# implementations (e.g. Open MPI 4.x) pull in C++ bindings from mpi.h.
find_package(MPI REQUIRED COMPONENTS CXX)

add_executable(info_set_json_example main.cpp)
target_compile_features(info_set_json_example PRIVATE cxx_std_17)
target_link_libraries(info_set_json_example PRIVATE
    mpi-extensions::mpi-extensions
    MPI::MPI_CXX
)
