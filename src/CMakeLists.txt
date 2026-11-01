add_library(mpi-extensions SHARED)

target_sources(mpi-extensions
    PRIVATE
        MPI_Dims_weighted_create.c
        MPI_Info_set_json.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            mpi-extensions.h
            MPI_Dims_weighted_create.h
            MPI_Info_set_json.h
)

target_link_libraries(mpi-extensions
    PRIVATE
        json-c::json-c
    PUBLIC
        MPI::MPI_C
)

install(TARGETS mpi-extensions
    EXPORT mpi-extensionsTargets
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
