# Setup the search paths
xmake_import_search_paths(ATHENA_SCRIPTING)
xmake_import_search_paths(ATHENA_CORE)


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

create_scripting_module(ATHENA_CORE_SCRIPTING_MODULE "Athena.Core" "${JS_SRCS}")
