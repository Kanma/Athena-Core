# List the source files
set(SRCS Signals/Signal.cpp
         Utils/Describable.cpp
         Utils/Path.cpp
         Utils/PropertiesList.cpp
         Utils/StringsMap.cpp
         Utils/StringUtils.cpp
         Utils/StringConverter.cpp
         Utils/Variant.cpp
)

if (APPLE)
    set(PLATFORM_SRCS Utils/OSX/Timer.cpp)
endif()

# Precompiler definitions
add_definitions(-DATHENA_EXPORTS)

if (ATHENA_DOUBLE_PRECISION)
    add_definitions(-DATHENA_DOUBLE_PRECISION=1)
else()
    add_definitions(-DATHENA_DOUBLE_PRECISION=0)
endif()

if (ATHENA_CORE_SCRIPTING)
    add_definitions(-DATHENA_CORE_SCRIPTING=1)
else()
    add_definitions(-DATHENA_CORE_SCRIPTING=0)
endif()

# List the include paths
include_directories(${ATHENA_CORE_SOURCE_DIR}/include ${ATHENA_MATH_SOURCE_DIR}/include)

# Declaration of the library
if (NOT ATHENA_CORE_STATIC)
    add_library(Athena-Core SHARED ${SRCS} ${PLATFORM_SRCS})
    target_link_libraries(Athena-Core Athena-Math)
else()
    add_library(Athena-Core STATIC ${SRCS} ${PLATFORM_SRCS})
endif()
