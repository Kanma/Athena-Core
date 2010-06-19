cmake_minimum_required(VERSION 2.6)
project(ATHENA_CORE)

# Settings
if (NOT DEFINED ATHENA_CORE_SCRIPTING)
    set(ATHENA_CORE_SCRIPTING OFF CACHE BOOL "Enable scripting support (needs Athena-Scripting)")
endif()

if (NOT DEFINED ATHENA_CORE_STATIC)
    set(ATHENA_CORE_STATIC OFF CACHE BOOL "Build Athena-Core as a static library")
endif()

set(ATHENA_CORE_DETAILED_LOGS ON CACHE BOOL "Enable detailed logs")

# Set the output paths
if (NOT DEFINED ATHENA_BINARY_DIR)
    set(ATHENA_BINARY_DIR "${ATHENA_CORE_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ATHENA_CORE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ATHENA_CORE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ATHENA_CORE_BINARY_DIR}/bin")
endif()

# Set the dependencies path
if (NOT DEFINED ATHENA_DEPENDENCIES_DIR)
    set(ATHENA_DEPENDENCIES_DIR "${ATHENA_CORE_SOURCE_DIR}/dependencies")
endif()

# Setup the dependencies
if (NOT DEFINED ATHENA_MATH_STATIC)
    set(ATHENA_MATH_STATIC ON)
endif()

# Import XMake
if (NOT EXISTS "${ATHENA_DEPENDENCIES_DIR}/xmake/XMake.cmake")
    message(FATAL_ERROR
"Missing dependency: XMake
Athena-Core requires the following dependencies:
   - XMake
   - Athena-Math
   - UnitTest++
They are provided as GIT submodules of this repository.
Did you forgot to execute the following commands?
   git submodule init
   git submodule update")
endif()

include("${ATHENA_DEPENDENCIES_DIR}/xmake/XMake.cmake")

# Process the dependencies
add_subdirectory(dependencies)

# XMake settings
if (ATHENA_CORE_STATIC)
    xmake_init_static(ATHENA_CORE)
    xmake_add_link_paths(ATHENA_CORE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    xmake_add_link_targets(ATHENA_CORE Athena-Core "${XMAKE_ATHENA_MATH_LINK_TARGETS}")
else()
    xmake_init_dynamic(ATHENA_CORE)
    xmake_add_link_paths(ATHENA_CORE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    xmake_add_link_targets(ATHENA_CORE Athena-Core)
endif()

xmake_add_include_paths(ATHENA_CORE ${ATHENA_CORE_SOURCE_DIR}/include
                                    ${XMAKE_ATHENA_MATH_INCLUDE_PATHS})

# Subdirectories to process
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(unittests)
