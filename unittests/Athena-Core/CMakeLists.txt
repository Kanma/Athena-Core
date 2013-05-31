# Setup the search paths
xmake_import_search_paths(ATHENA_CORE)
xmake_import_search_paths(UNITTEST_CPP)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_import_search_paths(ATHENA_SCRIPTING)
endif()


# List the header files
set(HEADERS Utils.h
            mocks/Describable.h
            mocks/LogListener.h
)


# List the source files
set(SRCS main.cpp
         Utils.cpp
         tests/test_ConfigFile.cpp
         tests/test_Describable.cpp
         tests/test_FileDataStream.cpp
         tests/test_Iterators.cpp
         tests/test_LocationManager.cpp
         tests/test_LogManager.cpp
         tests/test_PropertiesList.cpp
         tests/test_Signal.cpp
         tests/test_SignalsList.cpp
         tests/test_SignalsUtils.cpp
         tests/test_StringsMap.cpp
         tests/test_StringUtils.cpp
         tests/test_Timer.cpp
         tests/test_Variant.cpp
)


if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(HEADERS ${HEADERS} environments/ScriptingTestEnvironment.h)

    set(SRCS ${SRCS}
             module/module.cpp
             module/Describable.cpp
             tests/scripting.cpp
    )
endif()


# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_CORE UnitTests-Athena-Core ${HEADERS} ${SRCS})

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_SCRIPTS_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/Athena-Core/scripts/\";ATHENA_SCRIPTING_JS_TEST_RUNNER_PATH=\"${ATHENA_SCRIPTING_SOURCE_DIR}/unittests/js-test-runner/\"")

xmake_project_link(UNITTESTS_ATHENA_CORE UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_CORE ATHENA_CORE)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_project_link(UNITTESTS_ATHENA_CORE ATHENA_SCRIPTING)

    add_dependencies(UnitTests-Athena-Core Athena-Core-Scripting-module Run-UnitTests-Athena-Scripting)
endif()

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_DATA_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/Athena-Core/data/\"")


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
