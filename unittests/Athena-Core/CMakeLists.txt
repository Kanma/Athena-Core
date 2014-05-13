# Setup the search paths
xmake_import_search_paths(ATHENA_CORE)
xmake_import_search_paths(UNITTEST_CPP)


# List the header files
set(HEADERS mocks/Describable.h
            mocks/LogListener.h
)


# List the source files
set(SRCS main.cpp
         tests/test_Describable.cpp
         tests/test_FileDataStream.cpp
         tests/test_Iterators.cpp
         tests/test_LocationManager.cpp
         tests/test_LogManager.cpp
         tests/test_Path.cpp
         tests/test_PropertiesList.cpp
         tests/test_Signal.cpp
         tests/test_SignalsList.cpp
         tests/test_SignalsUtils.cpp
         tests/test_StringsMap.cpp
         tests/test_StringUtils.cpp
         tests/test_Timer.cpp
         tests/test_Variant.cpp
)


# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_CORE UnitTests-Athena-Core ${HEADERS} ${SRCS})

xmake_project_link(UNITTESTS_ATHENA_CORE UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_CORE ATHENA_CORE)


file(MAKE_DIRECTORY "${XMAKE_BINARY_DIR}/generated/unittests/Athena-Core/")

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_DATA_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/Athena-Core/data/\"")
xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_GENERATED_PATH=\"${XMAKE_BINARY_DIR}/generated/unittests/Athena-Core/\"")


# Run the unit tests
set(WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
if (WIN32)
    set(WORKING_DIRECTORY "${WORKING_DIRECTORY}/$(OutDir)")
endif()

add_custom_target(Run-UnitTests-Athena-Core ALL UnitTests-Athena-Core
                  DEPENDS UnitTests-Athena-Core
                  WORKING_DIRECTORY ${WORKING_DIRECTORY}
                  COMMENT "Unit testing: Athena-Core..."
                  VERBATIM)
