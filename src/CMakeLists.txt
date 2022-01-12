target_sources(mpi-extensions
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Dims_weighted_create.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/MPI_Dims_weighted_create.h
)
install(FILES MPI_Dims_weighted_create.h DESTINATION include)
