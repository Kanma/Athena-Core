# List the source files
set(SRCS Data/ConfigFile.cpp
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

# Precompiler definitions
add_definitions(-DATHENA_EXPORTS)

# List the include paths
xmake_include_directories(ATHENA_CORE)

# Declaration of the library
if (NOT ATHENA_CORE_STATIC)
    add_library(Athena-Core SHARED ${SRCS} ${PLATFORM_SRCS})
    xmake_target_link_libraries(Athena-Core ATHENA_MATH)

    # Set the INSTALL_PATH so that the dynamic library can be local
    set_target_properties(Athena-Core PROPERTIES BUILD_WITH_INSTALL_RPATH ON
                                                 INSTALL_NAME_DIR "@rpath"
    )
else()
    add_library(Athena-Core STATIC ${SRCS} ${PLATFORM_SRCS})
endif()
