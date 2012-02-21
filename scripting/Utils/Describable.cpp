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

#define GetPtr(HANDLE) GetObjectPtr<Describable>(HANDLE)


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

    Describable* self = GetPtr(args.This());
    assert(self);

    Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate("Athena.Utils.PropertiesList");

    Handle<Object> jsList = func->GetFunction()->NewInstance();
    SetObjectPtr(jsList, self->getProperties());

    return handle_scope.Close(jsList);
}

//-----------------------------------------------------------------------

Handle<Value> Describable_SetProperties(const Arguments& args)
{
    HandleScope handle_scope;

    Describable* self = GetPtr(args.This());
    assert(self);

    PropertiesList* pProperties = 0;

    if (args.Length() == 1)
        GetObjectPtr(args[0], &pProperties);

    if (!pProperties)
        return ThrowException(String::New("Invalid parameter, valid syntax:\nsetProperties(list)"));

    PropertiesList* pDelayedProperties = new PropertiesList();
    self->setProperties(pProperties, pDelayedProperties);

    if (pDelayedProperties->getCategoriesIterator().hasMoreElements())
    {
        Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate("Athena.Utils.PropertiesList");

        Handle<Object> jsList = func->GetFunction()->NewInstance();
        SetObjectPtr(jsList, pDelayedProperties);

        return handle_scope.Close(jsList);
    }
    
    delete pDelayedProperties;

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> Describable_GetUnknownProperties(const Arguments& args)
{
    HandleScope handle_scope;

    Describable* self = GetPtr(args.This());
    assert(self);

    PropertiesList* pProperties = self->getUnknownProperties();

    if (pProperties)
    {
        Handle<FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate("Athena.Utils.PropertiesList");

        Handle<Object> jsList = func->GetFunction()->NewInstance();
        SetObjectPtr(jsList, pProperties);

        return handle_scope.Close(jsList);
    }

    return Handle<Value>();
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
        AddMethod(describable, "properties",        Describable_GetProperties);
        AddMethod(describable, "setProperties",     Describable_SetProperties);
        AddMethod(describable, "unknownProperties", Describable_GetUnknownProperties);

        pManager->declareClassTemplate("Athena.Utils.Describable", describable);
    }

    // Add the class to the parent
    return parent->Set(String::New("Describable"), describable->GetFunction());
}
