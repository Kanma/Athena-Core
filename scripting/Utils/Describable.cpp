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
#include <Athena-Scripting/Conversions.h>
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
    // Wrapper around an existing C++ describable
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Describable* pDescribable = static_cast<Describable*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pDescribable, &NoOpWeakCallback);
    }
    else if (args.Length() == 0)
    {
        return SetObjectPtr(args.This(), new Describable());
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nDescribable()\nDescribable(<C++ describable>)"));
}


/**************************************** METHODS ***************************************/

Handle<Value> Describable_GetProperties(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Describable* self = GetPtr(info.This());
    assert(self);

    return handle_scope.Close(toJavaScript(self->getProperties()));
}

//-----------------------------------------------------------------------

void Describable_SetProperties(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Describable* self = GetPtr(info.This());
    assert(self);

    PropertiesList* pProperties = fromJSPropertiesList(value);

    self->setProperties(pProperties);
}

//-----------------------------------------------------------------------

Handle<Value> Describable_GetUnknownProperties(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Describable* self = GetPtr(info.This());
    assert(self);

    PropertiesList* pProperties = self->getUnknownProperties();

    if (pProperties)
        return handle_scope.Close(toJavaScript(pProperties));

    return Handle<Value>();
}


/**************************************** METHODS ***************************************/

Handle<Value> Describable_SetPropertiesEx(const Arguments& args)
{
    HandleScope handle_scope;

    Describable* self = GetPtr(args.This());
    assert(self);

    if ((args.Length() != 1) || !args[0]->IsObject())
        return ThrowException(String::New("Invalid parameters, valid syntax:\nsetProperties(properties_list)"));

    PropertiesList* pProperties = fromJSPropertiesList(args[0]);

    Handle<Object> jsDelayedProperties = createJSPropertiesList();
    PropertiesList* pDelayedProperties = fromJSPropertiesList(jsDelayedProperties);

    self->setProperties(pProperties, pDelayedProperties);

    if (pDelayedProperties->getCategoriesIterator().hasMoreElements())
        return handle_scope.Close(jsDelayedProperties);

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

        // Attributes
        AddAttribute(describable, "properties",         Describable_GetProperties, Describable_SetProperties);
        AddAttribute(describable, "unknownProperties",  Describable_GetUnknownProperties, 0);

        // Methods
        AddMethod(describable, "setProperties",         Describable_SetPropertiesEx);

        pManager->declareClassTemplate("Athena.Utils.Describable", describable);
    }

    // Add the class to the parent
    return parent->Set(String::New("Describable"), describable->GetFunction());
}
