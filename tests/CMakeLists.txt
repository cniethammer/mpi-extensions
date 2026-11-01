find_package(Catch2 3 REQUIRED)

include(Catch)
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

add_executable(mpi_dims_weighted_create_tests
    MPI_Dims_weighted_create_test.cpp
)

target_link_libraries(mpi_dims_weighted_create_tests
    Catch2::Catch2
    mpi-extensions
)
catch_discover_tests(mpi_dims_weighted_create_tests)


add_executable(mpi_info_set_json_tests
    MPI_Info_set_json_test.cpp
)
target_link_libraries(mpi_info_set_json_tests
    Catch2::Catch2
    mpi-extensions
)
catch_discover_tests(mpi_info_set_json_tests)

