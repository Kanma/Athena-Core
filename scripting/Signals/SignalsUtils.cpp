/** @file   SignalsUtils.cpp
    @author Philip Abbet
    
    Javascript binding of the class Athena::Signals::SignalsUtils
*/

#include <Athena-Core/Signals/SignalsUtils.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace Athena::Scripting;
using namespace v8;


/*************************************** FUNCTIONS *************************************/

Handle<Value> SignalsUtils_GetSignalID(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() != 1) || !args[0]->IsString())
        return ThrowException(String::New("Invalid parameters, valid syntax: signalID(name)"));

    return handle_scope.Close(Uint32::New(SignalsUtils::getSignalID(*String::AsciiValue(args[0]->ToString()))));
}

//-----------------------------------------------------------------------

Handle<Value> SignalsUtils_GetSignalName(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() != 1) || !args[0]->IsUint32())
        return ThrowException(String::New("Invalid parameters, valid syntax: signalName(id)"));

    return handle_scope.Close(String::New(SignalsUtils::getSignalName(args[0]->ToUint32()->Value()).c_str()));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Signals_SignalsUtils(Handle<Object> parent)
{
    // Add the functions to the parent
    return parent->Set(String::New("signalID"), FunctionTemplate::New(SignalsUtils_GetSignalID)->GetFunction()) &&
           parent->Set(String::New("signalName"), FunctionTemplate::New(SignalsUtils_GetSignalName)->GetFunction());
}
