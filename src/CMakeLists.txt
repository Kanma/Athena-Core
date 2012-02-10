# File generation
configure_file(${ATHENA_CORE_SOURCE_DIR}/src/module.cpp.in
               ${XMAKE_BINARY_DIR}/generated/Athena-Core/module.cpp)

# List the source files
set(SRCS ${XMAKE_BINARY_DIR}/generated/Athena-Core/module.cpp
         Data/ConfigFile.cpp
         Data/DataStream.cpp
         Data/FileDataStream.cpp
         Log/LogManager.cpp
         Log/XMLLogListener.cpp
         Signals/Signal.cpp
         Signals/SignalsList.cpp
         Signals/SignalsUtils.cpp
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
elseif (UNIX)
    set(PLATFORM_SRCS Utils/OSX/Timer.cpp)
endif()


# List the include paths
set(INCLUDE_PATHS "${ATHENA_CORE_SOURCE_DIR}/include"
                  "${XMAKE_BINARY_DIR}/include")

include_directories(${INCLUDE_PATHS})

xmake_import_search_paths(ATHENA_MATH)


# Declaration of the library
if (NOT ATHENA_CORE_STATIC)
    xmake_create_dynamic_library(ATHENA_CORE Athena-Core
                                "${ATHENA_CORE_VERSION_MAJOR}.${ATHENA_CORE_VERSION_MINOR}.${ATHENA_CORE_VERSION_PATCH}"
                                "${ATHENA_CORE_VERSION_MAJOR}.${ATHENA_CORE_VERSION_MINOR}.${ATHENA_CORE_VERSION_PATCH}"
                                 ${SRCS} ${PLATFORM_SRCS})
else()
    xmake_create_static_library(ATHENA_CORE Athena-Core ${SRCS} ${PLATFORM_SRCS})
endif()

xmake_project_link(ATHENA_CORE ATHENA_MATH)

xmake_add_to_list_property(ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_EXPORTS")


# Exportations
xmake_export_include_paths(ATHENA_CORE ${INCLUDE_PATHS})
