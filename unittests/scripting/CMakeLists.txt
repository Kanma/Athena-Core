if (NOT DEFINED ATHENA_SCRIPTING_ENABLED OR NOT ATHENA_SCRIPTING_ENABLED)
    message(FATAL_ERROR "Scripting not enabled, can't compile the scripting-related unit tests of Athena-Core")
endif()


# Setup the search paths
xmake_import_search_paths(ATHENA_CORE)
xmake_import_search_paths(UNITTEST_CPP)
xmake_import_search_paths(ATHENA_SCRIPTING)


# List the header files
set(HEADERS ../Athena-Core/mocks/Describable.h
            # ../Athena-Core/mocks/LogListener.h
            environments/ScriptingTestEnvironment.h
)


# List the source files
set(SRCS main.cpp
         module/module.cpp
         module/Describable.cpp
         tests/scripting.cpp
)


# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_CORE_SCRIPTING UnitTests-Athena-Core-Scripting ${HEADERS} ${SRCS})

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE_SCRIPTING COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_SCRIPTS_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/scripting/scripts/\";ATHENA_SCRIPTING_JS_TEST_RUNNER_PATH=\"${ATHENA_SCRIPTING_SOURCE_DIR}/unittests/js-test-runner/\"")

xmake_project_link(UNITTESTS_ATHENA_CORE_SCRIPTING UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_CORE_SCRIPTING ATHENA_CORE)
xmake_project_link(UNITTESTS_ATHENA_CORE_SCRIPTING ATHENA_SCRIPTING)

add_dependencies(UnitTests-Athena-Core-Scripting Athena-Core-Scripting-module Run-UnitTests-Athena-Scripting Run-UnitTests-Athena-Core)


# Run the unit tests
set(WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
if (WIN32)
    set(WORKING_DIRECTORY "${WORKING_DIRECTORY}/$(OutDir)")
endif()

add_custom_target(Run-UnitTests-Athena-Core-Scripting ALL UnitTests-Athena-Core-Scripting
                  DEPENDS UnitTests-Athena-Core-Scripting
                  WORKING_DIRECTORY ${WORKING_DIRECTORY}
                  COMMENT "Unit testing: Athena-Core-Scripting..."
                  VERBATIM)
