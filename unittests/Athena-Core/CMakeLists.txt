# Setup the search paths
include_directories(${ATHENA_CORE_SOURCE_DIR}/include ${ATHENA_MATH_SOURCE_DIR}/include ${UNITTEST_CPP_SOURCE_DIR}/include)

# List the source files
file(GLOB SRCS main.cpp
               test_Iterators.cpp
               test_PropertiesList.cpp
               test_StringsMap.cpp
               test_StringUtils.cpp
               test_Variant.cpp
)

# Create and link the executable
add_executable(UnitTests-Athena-Core ${SRCS})
target_link_libraries(UnitTests-Athena-Core UnitTest++ Athena-Core)

# Run the unit tests
get_property(PATH TARGET UnitTests-Athena-Core PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Run-UnitTests-Athena-Core ALL UnitTests-Athena-Core DEPENDS UnitTests-Athena-Core WORKING_DIRECTORY ${PATH} COMMENT "Unit testing: Athena-Core..." VERBATIM)
