cmake_minimum_required(VERSION 3.5)
project(mpi-extensions)

cmake_policy(SET CMP0110 NEW)

find_package(MPI REQUIRED)

include(CTest)

add_library(mpi-extensions SHARED)
install(TARGETS mpi-extensions DESTINATION lib)
install(FILES src/mpi-extensions.h DESTINATION include)

add_subdirectory(src)
add_subdirectory(tests)

