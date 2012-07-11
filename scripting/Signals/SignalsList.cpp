/** @file   SignalsList.cpp
    @author Philip Abbet

    Javascript binding of the class Athena::Signals::SignalsList
*/

#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetObjectPtr(HANDLE) GetObjectPtr<SignalsList>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> SignalsList_New(const Arguments& args)
{
    // New C++ list
    if (args.Length() == 0)
    {
        return SetObjectPtr(args.This(), new SignalsList());
    }

    // Wrapper around an existing C++ list
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        SignalsList* pList = static_cast<SignalsList*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pList, &NoOpWeakCallback);
    }

    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nSignalsList()\nSignalsList(<C++ signals list>)"));
    }
}


/************************************** PROPERTIES *************************************/

Handle<Value> SignalsList_IsEmpty(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    SignalsList* self = GetObjectPtr(info.This());
    assert(self);

    return handle_scope.Close(Boolean::New(self->isEmpty()));
}


/**************************************** METHODS ***************************************/

Handle<Value> SignalsList_Connect(const Arguments& args)
{
    HandleScope handle_scope;

    SignalsList* self = GetObjectPtr(args.This());
    assert(self);

    if ((args.Length() == 3) && args[0]->IsUint32() && args[1]->IsObject() && args[2]->IsFunction())
    {
        self->connect(args[0]->ToUint32()->Value(),
                      Persistent<Object>::New(args[1]->ToObject()),
                      Persistent<Object>::New(args[2]->ToObject()));
    }
    else if ((args.Length() == 2) && args[0]->IsUint32() && args[1]->IsFunction())
    {
        self->connect(args[0]->ToUint32()->Value(),
                      Persistent<Object>::New(args[1]->ToObject()));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nconnect(id, object, function)\nconnect(id, function)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> SignalsList_Disconnect(const Arguments& args)
{
    HandleScope handle_scope;

    SignalsList* self = GetObjectPtr(args.This());
    assert(self);

    if ((args.Length() == 3) && args[0]->IsUint32() && args[1]->IsObject() && args[2]->IsFunction())
    {
        self->disconnect(args[0]->ToUint32()->Value(),
                         Persistent<Object>::New(args[1]->ToObject()),
                         Persistent<Object>::New(args[2]->ToObject()));
    }
    else if ((args.Length() == 2) && args[0]->IsUint32() && args[1]->IsFunction())
    {
        self->disconnect(args[0]->ToUint32()->Value(),
                         Persistent<Object>::New(args[1]->ToObject()));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\ndisconnect(id, object, function)\ndisconnect(id, function)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> SignalsList_Fire(const Arguments& args)
{
    HandleScope handle_scope;

    SignalsList* self = GetObjectPtr(args.This());
    assert(self);

    Variant* pValue = 0;
    unsigned int id = 0;

    if (((args.Length() == 1) || (args.Length() == 2)) && args[0]->IsUint32())
        id = args[0]->ToUint32()->Value();
    else
        return ThrowException(String::New("Invalid parameters, valid syntax:\nfire(id, value)\nfire(id)"));

    if (args.Length() == 2)
        pValue = fromJSVariant(args[1]);

    self->fire(id, pValue);

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Signals_SignalsList(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> signalsList = pManager->getClassTemplate("Athena.Signals.SignalsList");

    if (signalsList.IsEmpty())
    {
        // Declaration of the class
        signalsList = FunctionTemplate::New(SignalsList_New);
        signalsList->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(signalsList, "empty",   SignalsList_IsEmpty, 0);

        // Methods
        AddMethod(signalsList, "connect",    SignalsList_Connect);
        AddMethod(signalsList, "disconnect", SignalsList_Disconnect);
        AddMethod(signalsList, "fire",       SignalsList_Fire);

        pManager->declareClassTemplate("Athena.Signals.SignalsList", signalsList);
    }

    // Add the class to the parent
    return parent->Set(String::New("SignalsList"), signalsList->GetFunction());
}
