/** @file   Signal.cpp
    @author Philip Abbet

    Javascript binding of the class Athena::Signals::Signal
*/

#include <Athena-Core/Signals/Signal.h>
#include <Athena-Core/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Signal>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Signal_New(const Arguments& args)
{
    // Wrapper around an existing C++ entity
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Signal* pSignal = static_cast<Signal*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pSignal, &NoOpWeakCallback);
    }
    else if (args.Length() == 0)
    {
        return SetObjectPtr(args.This(), new Signal());
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nSignal()\nSignal(<C++ signal>)"));
}


/************************************** PROPERTIES *************************************/

Handle<Value> Signal_IsDisconnected(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Signal* self = GetPtr(info.This());
    assert(self);

    return handle_scope.Close(Boolean::New(self->isDisconnected()));
}


/**************************************** METHODS ***************************************/

Handle<Value> Signal_Connect(const Arguments& args)
{
    HandleScope handle_scope;

    Signal* self = GetPtr(args.This());
    assert(self);

    if ((args.Length() == 2) && args[0]->IsObject() && args[1]->IsFunction())
    {
        self->connect(Persistent<Object>::New(args[0]->ToObject()),
                      Persistent<Object>::New(args[1]->ToObject()));
    }
    else if ((args.Length() == 1) && args[0]->IsFunction())
    {
        self->connect(Persistent<Object>::New(args[0]->ToObject()));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nconnect(object, function)\nconnect(function)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Signal_Disconnect(const Arguments& args)
{
    HandleScope handle_scope;

    Signal* self = GetPtr(args.This());
    assert(self);

    if ((args.Length() == 2) && args[0]->IsObject() && args[1]->IsFunction())
    {
        self->disconnect(Persistent<Object>::New(args[0]->ToObject()),
                         Persistent<Object>::New(args[1]->ToObject()));
    }
    else if ((args.Length() == 1) && args[0]->IsFunction())
    {
        self->disconnect(Persistent<Object>::New(args[0]->ToObject()));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\ndisconnect(object, function)\ndisconnect(function)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Signal_Fire(const Arguments& args)
{
    HandleScope handle_scope;

    Signal* self = GetPtr(args.This());
    assert(self);

    Variant* pValue = 0;

    if (args.Length() == 1)
        pValue = fromJS(args[0]);

    self->fire(pValue);

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Signals_Signal(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> signal = pManager->getClassTemplate("Athena.Signals.Signal");

    if (signal.IsEmpty())
    {
        // Declaration of the class
        signal = FunctionTemplate::New(Signal_New);
        signal->InstanceTemplate()->SetInternalFieldCount(1);

        // Attributes
        AddAttribute(signal, "disconnected", Signal_IsDisconnected, 0);

        // Methods
        AddMethod(signal, "connect",         Signal_Connect);
        AddMethod(signal, "disconnect",      Signal_Disconnect);
        AddMethod(signal, "fire",            Signal_Fire);

        pManager->declareClassTemplate("Athena.Signals.Signal", signal);
    }

    // Add the class to the parent
    return parent->Set(String::New("Signal"), signal->GetFunction());
}
