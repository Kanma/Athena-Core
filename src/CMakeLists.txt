# File generation
configure_file(${ATHENA_CORE_SOURCE_DIR}/src/module.cpp.in
               ${XMAKE_BINARY_DIR}/generated/Athena-Core/module.cpp)

# List the header files
set(HEADERS ${XMAKE_BINARY_DIR}/include/Athena-Core/Config.h
            ../include/Athena-Core/Prerequisites.h
            ../include/Athena-Core/Data/ConfigFile.h
            ../include/Athena-Core/Data/DataStream.h
            ../include/Athena-Core/Data/FileDataStream.h
            ../include/Athena-Core/Data/GenericDataStream.h
            ../include/Athena-Core/Log/Declarations.h
            ../include/Athena-Core/Log/ILogListener.h
            ../include/Athena-Core/Log/LogManager.h
            ../include/Athena-Core/Log/XMLLogListener.h
            ../include/Athena-Core/Signals/Declarations.h
            ../include/Athena-Core/Signals/Signal.h
            ../include/Athena-Core/Signals/SignalsList.h
            ../include/Athena-Core/Signals/SignalsUtils.h
            ../include/Athena-Core/Utils/Describable.h
            ../include/Athena-Core/Utils/Iterators.h
            ../include/Athena-Core/Utils/Path.h
            ../include/Athena-Core/Utils/PropertiesList.h
            ../include/Athena-Core/Utils/Singleton.h
            ../include/Athena-Core/Utils/StringConverter.h
            ../include/Athena-Core/Utils/StringsMap.h
            ../include/Athena-Core/Utils/StringUtils.h
            ../include/Athena-Core/Utils/Timer.h
            ../include/Athena-Core/Utils/Variant.h
)

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

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(HEADERS ${HEADERS} ../include/Athena-Core/Scripting.h)
    set(SRCS ${SRCS} Scripting.cpp)
endif()


if (APPLE)
    set(PLATFORM_HEADERS ../include/Athena-Core/Utils/OSX/Timer.h)
    set(PLATFORM_SRCS Utils/OSX/Timer.cpp)
elseif (UNIX)
    set(PLATFORM_HEADERS ../include/Athena-Core/Utils/OSX/Timer.h)
    set(PLATFORM_SRCS Utils/OSX/Timer.cpp)
elseif (WIN32)
    set(PLATFORM_HEADERS ../include/Athena-Core/Utils/WIN32/Timer.h)
    set(PLATFORM_SRCS Utils/WIN32/Timer.cpp)
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
                                 ${HEADERS} ${SRCS} ${PLATFORM_HEADERS} ${PLATFORM_SRCS})

    xmake_add_to_list_property(ATHENA_CORE COMPILE_DEFINITIONS "ATHENA_CORE_EXPORTS")
else()
    xmake_create_static_library(ATHENA_CORE Athena-Core ${HEADERS} ${SRCS} ${PLATFORM_HEADERS} ${PLATFORM_SRCS})
endif()

xmake_project_link(ATHENA_CORE ATHENA_MATH)


# Disable some warnings in Visual Studio
xmake_add_to_list_property(ATHENA_CORE COMPILE_DEFINITIONS "_CRT_SECURE_NO_WARNINGS")
xmake_disable_vs_warning(ATHENA_CORE 4251 YES)
xmake_disable_vs_warning(ATHENA_CORE 4661 YES)


# Exportations
xmake_export_include_paths(ATHENA_CORE ${INCLUDE_PATHS})
