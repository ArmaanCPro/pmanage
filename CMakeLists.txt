cmake_minimum_required(VERSION 3.30)

# See https://cmake.org/cmake/help/latest/policy/CMP0077.html
# This allows for setting option variables externally, when this project
# is included in another CMake project.
cmake_policy(SET CMP0077 NEW)

# --- Project
project (
    pmanage
    DESCRIPTION "OS Process Management Abstraction for C++"
    LANGUAGES CXX
    VERSION 0.1.0
)

option(PMANAGE_BUILD_TESTS "Build unit tests" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(CommonOptions)
include(Sanitizers)
include(IPO)

# --- Testing
if (PMANAGE_BUILD_TESTS)
    enable_testing()
    include(CTest)
    add_subdirectory(test)
endif()

add_library(pmanage SHARED)

target_compile_definitions(pmanage
    PRIVATE PMANAGE_EXPORTS
)

target_sources(pmanage
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS src
            FILES
                src/common.h

        FILE_SET cxx_modules TYPE CXX_MODULES
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
            FILES
                src/process.cppm
    PRIVATE
        $<$<PLATFORM_ID:Windows>:src/platform/process_win32.cpp>
        $<$<NOT:$<PLATFORM_ID:Windows>>:src/platform/process_posix.cpp>
)

target_include_directories(pmanage
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(pmanage
    PRIVATE pmanageCommon
)

set_target_properties(pmanage PROPERTIES
    EXPORT_NAME pmanage
)

enable_sanitizers(pmanage)
enable_ipo(pmanage)
set_target_properties(pmanage PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

add_library(pmanage::pmanage ALIAS pmanage)

install(
    TARGETS
        pmanage
        pmanageCommon
    EXPORT pmanageTargets

    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET cxx_modules DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}

    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT pmanageTargets
    NAMESPACE pmanage::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pmanage
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pmanageConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/pmanageConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pmanage
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/pmanageConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pmanageConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/pmanageConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pmanage
)

