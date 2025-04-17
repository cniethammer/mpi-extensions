cmake_minimum_required(VERSION 3.23)
project(mpi-extensions)

find_package(MPI REQUIRED)
include_directories( ${MPI_C_INCLUDE_DIRS} )

find_package(json-c REQUIRED)


add_library(mpi-extensions SHARED)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)


add_subdirectory(src)
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(tests)
endif()

install(TARGETS mpi-extensions
    EXPORT mpi-extensionsTargets
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT mpi-extensionsTargets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE mpi-extensions::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mpi-extensions
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/mpi-extensionsConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mpi-extensions
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mpi-extensionsConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mpi-extensions
)
