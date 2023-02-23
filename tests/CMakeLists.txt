option(FIND_PACKAGE_CATCH2 "Uses find_package to find installed catch2 instead of fetching it from github." ON)

if (FIND_PACKAGE_CATCH2)
    find_package(Catch2 REQUIRED)
else ()
    include(FetchContent)
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v2.13.7
    )
    FetchContent_MakeAvailable(Catch2)
    # add cmake module path to make later include work
    # see https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
endif ()

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
target_include_directories(mpi_dims_weighted_create_tests PRIVATE
    ${MPI_CXX_INCLUDE_DIRS}
    ../src
)

catch_discover_tests(mpi_dims_weighted_create_tests)

