/** @file   Timer.cpp
    @author Philip Abbet

    Javascript binding of the class Athena::Utils::Timer
*/

#include <Athena-Core/Utils/Timer.h>
#include <Athena-Core/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Utils;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetObjectPtr(HANDLE) GetObjectPtr<Timer>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Timer_New(const Arguments& args)
{
    return SetObjectPtr(args.This(), new Timer());
}


/**************************************** METHODS ***************************************/

Handle<Value> Timer_Reset(const Arguments& args)
{
    HandleScope handle_scope;

    Timer* self = GetObjectPtr(args.This());
    assert(self);

    self->reset();

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Timer_GetMilliseconds(const Arguments& args)
{
    HandleScope handle_scope;

    Timer* self = GetObjectPtr(args.This());
    assert(self);

    return handle_scope.Close(Uint32::New(self->getMilliseconds()));
}

//-----------------------------------------------------------------------

Handle<Value> Timer_GetMicroseconds(const Arguments& args)
{
    HandleScope handle_scope;

    Timer* self = GetObjectPtr(args.This());
    assert(self);

    return handle_scope.Close(Uint32::New(self->getMicroseconds()));
}

//-----------------------------------------------------------------------

Handle<Value> Timer_GetMillisecondsCPU(const Arguments& args)
{
    HandleScope handle_scope;

    Timer* self = GetObjectPtr(args.This());
    assert(self);

    return handle_scope.Close(Uint32::New(self->getMillisecondsCPU()));
}

//-----------------------------------------------------------------------

Handle<Value> Timer_GetMicrosecondsCPU(const Arguments& args)
{
    HandleScope handle_scope;

    Timer* self = GetObjectPtr(args.This());
    assert(self);

    return handle_scope.Close(Uint32::New(self->getMicrosecondsCPU()));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Utils_Timer(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> timer = pManager->getClassTemplate("Athena.Utils.Timer");

    if (timer.IsEmpty())
    {
        // Declaration of the class
        timer = FunctionTemplate::New(Timer_New);
        timer->InstanceTemplate()->SetInternalFieldCount(1);

        // Methods
        AddMethod(timer, "reset",           Timer_Reset);
        AddMethod(timer, "milliseconds",    Timer_GetMilliseconds);
        AddMethod(timer, "microseconds",    Timer_GetMicroseconds);
        AddMethod(timer, "millisecondsCPU", Timer_GetMillisecondsCPU);
        AddMethod(timer, "microsecondsCPU", Timer_GetMicrosecondsCPU);

        pManager->declareClassTemplate("Athena.Utils.Timer", timer);
    }

    // Add the class to the parent
    return parent->Set(String::New("Timer"), timer->GetFunction());
}
