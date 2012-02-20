/** @file   Describable.cpp
    @author Philip Abbet
    
    Javascript binding of the class Athena::Utils::Describable
*/

#include <Athena-Core/Utils/Describable.h>
#include <Athena-Core/Scripting.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Color.h>
#include <Athena-Math/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Utils;
using namespace Athena::Math;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetObjectPtr(HANDLE) GetObjectPtr<Describable>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> Describable_New(const Arguments& args)
{
    return SetObjectPtr(args.This(), new Describable());
}


/**************************************** METHODS ***************************************/

Handle<Value> Describable_GetProperties(const Arguments& args)
{
    HandleScope handle_scope;
    
    Describable* self = GetObjectPtr(args.This());
    assert(self);

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate("Athena.Utils.PropertiesList");

    Handle<Object> jsList = func->GetFunction()->NewInstance();
    SetObjectPtr(jsList, self->getProperties());

    return handle_scope.Close(jsList);
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Utils_Describable(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();
    
    Handle<FunctionTemplate> describable = pManager->getClassTemplate("Athena.Utils.Describable");
    
    if (describable.IsEmpty())
    {
        // Declaration of the class
        describable = FunctionTemplate::New(Describable_New);
        describable->InstanceTemplate()->SetInternalFieldCount(1);
        
        // Methods
        AddMethod(describable, "properties", Describable_GetProperties);

        pManager->declareClassTemplate("Athena.Utils.Describable", describable);
    }

    // Add the class to the parent
    return parent->Set(String::New("Describable"), describable->GetFunction());
}
