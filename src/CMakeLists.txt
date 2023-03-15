find_package(json-c CONFIG)
target_link_libraries(mpi-extensions PRIVATE json-c::json-c)

target_sources(mpi-extensions
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Dims_weighted_create.c
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Info_set_json.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Dims_weighted_create.h
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Info_set_json.h
)
install(FILES MPI_Dims_weighted_create.h DESTINATION include)
