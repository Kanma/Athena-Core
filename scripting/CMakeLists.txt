# Setup the search paths
xmake_import_search_paths(ATHENA_SCRIPTING)


# List the source files
set(SRCS module.cpp
         Signals/Signal.cpp
         Signals/SignalsList.cpp
         Signals/SignalsUtils.cpp
         Utils/Describable.cpp
         Utils/PropertiesList.cpp
         Utils/Timer.cpp
)

set(JS_SRCS ""
)


# Declaration of the module
xmake_create_dynamic_library(ATHENA_CORE_SCRIPTING_MODULE Athena-Core-Scripting-module
                            "${ATHENA_CORE_VERSION_MAJOR}.${ATHENA_CORE_VERSION_MINOR}.${ATHENA_CORE_VERSION_PATCH}"
                            "${ATHENA_CORE_VERSION_MAJOR}.${ATHENA_CORE_VERSION_MINOR}.${ATHENA_CORE_VERSION_PATCH}"
                             ${SRCS})

xmake_project_link(ATHENA_CORE_SCRIPTING_MODULE ATHENA_SCRIPTING V8)
set_target_properties(Athena-Core-Scripting-module PROPERTIES OUTPUT_NAME "Core"
                                                              PREFIX ""
                                                              LIBRARY_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin/modules/Athena")

# Copy of the JavaScript files
foreach (JS_SRC ${JS_SRCS})
    configure_file(${JS_SRC} "${XMAKE_BINARY_DIR}/bin/modules/Athena/js/Core/${JS_SRC}" COPYONLY)
endforeach()
