# List the source files
set(SRCS Describable.cpp
         Path.cpp
         PropertiesList.cpp
         StringsMap.cpp
         StringUtils.cpp
         StringConverter.cpp
         Variant.cpp
)

# Precompiler definitions
add_definitions(-DATHENA_EXPORTS)

if (ATHENA_DOUBLE_PRECISION)
    add_definitions(-DATHENA_DOUBLE_PRECISION=1)
else()
    add_definitions(-DATHENA_DOUBLE_PRECISION=0)
endif()

# List the include paths
include_directories(${ATHENA_CORE_SOURCE_DIR}/include ${ATHENA_MATH_SOURCE_DIR}/include)

# Declaration of the library
if (NOT ATHENA_CORE_STATIC)
    add_library(Athena-Core SHARED ${SRCS})
    target_link_libraries(Athena-Core Athena-Math)
else()
    add_library(Athena-Core STATIC ${SRCS})
endif()
