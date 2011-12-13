# Setup the search paths
xmake_import_search_paths(ATHENA_CORE)
xmake_import_search_paths(UNITTEST_CPP)


# List the source files
file(GLOB SRCS main.cpp
               test_ConfigFile.cpp
               test_Describable.cpp
               test_FileDataStream.cpp
               test_Iterators.cpp
               test_LogManager.cpp
               test_PropertiesList.cpp
               test_Signal.cpp
               test_SignalsList.cpp
               test_SignalsUtils.cpp
               test_StringsMap.cpp
               test_StringUtils.cpp
               test_Variant.cpp
               Utils.cpp
)



# Declaration of the executable
xmake_create_executable(UNITTESTS_ATHENA_CORE UnitTests-Athena-Core ${SRCS})

xmake_project_link(UNITTESTS_ATHENA_CORE UNITTEST_CPP)
xmake_project_link(UNITTESTS_ATHENA_CORE ATHENA_CORE)

xmake_add_to_list_property(UNITTESTS_ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_UNITTESTS_DATA_PATH=\"${ATHENA_CORE_SOURCE_DIR}/unittests/data/\"")


# Run the unit tests
get_property(PATH TARGET UnitTests-Athena-Core PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Run-UnitTests-Athena-Core ALL UnitTests-Athena-Core DEPENDS UnitTests-Athena-Core WORKING_DIRECTORY ${PATH} COMMENT "Unit testing: Athena-Core..." VERBATIM)
