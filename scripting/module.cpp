/** @file   module.cpp
    @author Philip Abbet

    Entry point of the Javascript module of Athena::Core
*/

#include <v8.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <string>

using namespace v8;
using namespace Athena::Scripting;


/*********************************** EXTERNAL FUNCTIONS *********************************/

// extern bool bind_Data_DataStream(Handle<Object> parent);

extern bool bind_Utils_Describable(Handle<Object> parent);


/*************************************** FUNCTIONS *************************************/

// bool init_data_submodule(Handle<Object> parent, const std::string& modulePath)
// {
//     HandleScope handle_scope;
// 
//     Handle<Object> ns = Object::New();
//     parent->Set(String::New("Data"), ns);
// 
//     return bind_Data_DataStream(ns);
// }


bool init_utils_submodule(Handle<Object> parent, const std::string& modulePath)
{
    HandleScope handle_scope;

    Handle<Object> ns = Object::New();
    parent->Set(String::New("Utils"), ns);

    return bind_Utils_Describable(ns);
}


/****************************** INITIALISATION OF THE MODULE ****************************/

extern "C" {

    bool init_module(Handle<Object> parent, const std::string& modulePath)
    {
        HandleScope handle_scope;

        parent->Set(String::New("VERSION"), String::New(Athena::Core::VERSION));

        Handle<Context> context = parent->CreationContext();
        Handle<Object> ns = context->Global()->Get(String::New("Athena"))->ToObject();

        return init_utils_submodule(ns, modulePath);
    }
}
