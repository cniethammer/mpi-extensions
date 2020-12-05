cmake_minimum_required(VERSION 3.1)
project(mpi-extensions)

cmake_policy(SET CMP0110 NEW)

find_package(MPI REQUIRED)
find_package(Catch2 REQUIRED)

include(CTest)
include(ParseAndAddCatchTests)
set(PARSE_CATCH_TESTS_NO_HIDDEN_TESTS on)

add_library(mpi-extensions SHARED)
install(TARGETS mpi-extensions DESTINATION lib)
install(FILES src/mpi-extensions.h DESTINATION include)

add_subdirectory(src)

