cmake_minimum_required(VERSION 3.28)
project(mpi-extensions
    VERSION 0.1
    DESCRIPTION "Extensions library for the MPI interface"
    LANGUAGES C CXX
)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

find_package(MPI REQUIRED)
find_package(json-c REQUIRED)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(src)

# package configuration for find_package(mpi-extensions)
install(EXPORT mpi-extensionsTargets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE mpi-extensions::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY SameMinorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
