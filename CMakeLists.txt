cmake_minimum_required(VERSION 2.6)
project(ATHENA_CORE)

##########################################################################################
# Settings

set(ATHENA_CORE_DETAILED_LOGS ON CACHE BOOL "Enable detailed logs")


##########################################################################################
# Library version

set(ATHENA_CORE_VERSION_MAJOR 1)
set(ATHENA_CORE_VERSION_MINOR 3)
set(ATHENA_CORE_VERSION_PATCH 3)
set(ATHENA_CORE_VERSION_SUFFIX "")


##########################################################################################
# XMake-related settings

# Set the output paths
if (NOT DEFINED XMAKE_BINARY_DIR)
    set(XMAKE_BINARY_DIR "${ATHENA_CORE_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin")
endif()

# Set the dependencies path
if (NOT DEFINED XMAKE_DEPENDENCIES_DIR)
    set(XMAKE_DEPENDENCIES_DIR "${ATHENA_CORE_SOURCE_DIR}/dependencies")
endif()


##########################################################################################
# XMake importation

if (NOT EXISTS "${XMAKE_DEPENDENCIES_DIR}/XMake/XMake.cmake")
    message(FATAL_ERROR
"Missing dependency: XMake
Athena-Core requires the following dependencies:
   - XMake
   - Athena-Math
   - UnitTest++
   - rapidjson
They are provided as GIT submodules of this repository.
Did you forgot to execute the following commands?
   git submodule init
   git submodule update")
endif()

include("${XMAKE_DEPENDENCIES_DIR}/XMake/XMake.cmake")


##########################################################################################
# Process subdirectories

add_subdirectory(dependencies)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(ATHENA_CORE_SCRIPTING ON)
else()
    set(ATHENA_CORE_SCRIPTING OFF)
endif()

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(unittests)
