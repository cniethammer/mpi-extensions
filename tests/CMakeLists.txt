find_package(Catch2 REQUIRED)

include(Catch)
set(PARSE_CATCH_TESTS_NO_HIDDEN_TESTS on)

add_executable(mpi_dims_weighted_create_tests
    MPI_Dims_weighted_create_test.cpp
)

target_link_libraries(mpi_dims_weighted_create_tests
    Catch2::Catch2
    mpi-extensions
    ${MPI_CXX_LIBRARIES}
)
target_include_directories(mpi_dims_weighted_create_tests PRIVATE ../src)

catch_discover_tests(mpi_dims_weighted_create_tests)

