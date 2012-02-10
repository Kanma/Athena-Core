/** @file   module.cpp
    @author Philip Abbet

    Entry point of the Javascript module of Athena::Core
*/

#include <v8.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <string>

using namespace v8;
using namespace Athena::Scripting;


/****************************** INITIALISATION OF THE MODULE ****************************/

extern "C" {

    bool init_module(Handle<Object> parent, const std::string& modulePath)
    {
        HandleScope handle_scope;

        parent->Set(String::New("VERSION"), String::New(Athena::Core::VERSION));

        return true;
    }
}
