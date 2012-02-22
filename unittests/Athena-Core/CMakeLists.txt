# Setup the search paths
xmake_import_search_paths(ATHENA_CORE)
xmake_import_search_paths(UNITTEST_CPP)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_import_search_paths(ATHENA_SCRIPTING)
endif()


# List the source files
set(SRCS main.cpp
         Utils.cpp
         tests/ConfigFile.cpp
         tests/Describable.cpp
         tests/FileDataStream.cpp
         tests/Iterators.cpp
         tests/LogManager.cpp
         tests/PropertiesList.cpp
         tests/Signal.cpp
         tests/SignalsList.cpp
         tests/SignalsUtils.cpp
         tests/StringsMap.cpp
         tests/StringUtils.cpp
         tests/Variant.cpp
)


if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(SRCS ${SRCS}
             module/module.cpp
             module/Describable.cpp
             tests/scripting.cpp
    )
endif()


# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_CORE UnitTests-Athena-Core ${SRCS})

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_SCRIPTS_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/Athena-Core/scripts/\";ATHENA_SCRIPTING_JS_TEST_RUNNER_PATH=\"${ATHENA_SCRIPTING_SOURCE_DIR}/unittests/js-test-runner/\"")

xmake_project_link(UNITTESTS_ATHENA_CORE UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_CORE ATHENA_CORE)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_project_link(UNITTESTS_ATHENA_CORE ATHENA_SCRIPTING)

    add_dependencies(UnitTests-Athena-Core Athena-Core-Scripting-module Run-UnitTests-Athena-Scripting)
endif()

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_DATA_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/data/\"")


# Run the unit tests
get_property(PATH TARGET UnitTests-Athena-Core PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Run-UnitTests-Athena-Core ALL UnitTests-Athena-Core
                  DEPENDS UnitTests-Athena-Core
                  WORKING_DIRECTORY ${PATH}
                  COMMENT "Unit testing: Athena-Core..."
                  VERBATIM)
