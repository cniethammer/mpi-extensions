target_link_libraries(mpi-extensions PRIVATE json-c::json-c)

install(FILES mpi-extensions.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

target_sources(mpi-extensions
    PRIVATE
        MPI_Dims_weighted_create.c
        MPI_Info_set_json.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            MPI_Dims_weighted_create.h
            MPI_Info_set_json.h
)

target_link_libraries(mpi-extensions
    PUBLIC
        ${MPI_CXX_LIBRARIES}
)
