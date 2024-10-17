cmake_minimum_required(VERSION 3.14)
project(mpi-extensions)

find_package(MPI REQUIRED)
include_directories( ${MPI_C_INCLUDE_DIRS} )

find_package(json-c REQUIRED)

include(CTest)

add_library(mpi-extensions SHARED)
install(TARGETS mpi-extensions DESTINATION lib)
install(FILES src/mpi-extensions.h DESTINATION include)

add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

