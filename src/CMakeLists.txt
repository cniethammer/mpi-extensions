target_sources(mpi-extensions
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Dims_weighted_create.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Dims_weighted_create.h
)

add_executable(mpi_dims_weighted_create_tests
    MPI_Dims_weighted_create_test.cpp
)

target_link_libraries(mpi_dims_weighted_create_tests
    Catch2::Catch2
    mpi-extensions
    ${MPI_CXX_LIBRARIES}
)

ParseAndAddCatchTests(mpi_dims_weighted_create_tests)

