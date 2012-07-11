/** @file   PropertiesList.cpp
    @author Philip Abbet

    Javascript binding of the class Athena::Utils::PropertiesList
*/

#include <Athena-Core/Utils/PropertiesList.h>
#include <Athena-Core/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Utils;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<PropertiesList>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> PropertiesList_New(const Arguments& args)
{
    // Wrapper around an existing C++ properties list
    if ((args.Length() == 1) && args[0]->IsExternal())
    {
        PropertiesList* pList = static_cast<PropertiesList*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pList, &NoOpWeakCallback);
    }
    else if (args.Length() == 0)
    {
        return SetObjectPtr(args.This(), new PropertiesList());
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nPropertiesList()\nPropertiesList(<C++ properties list>)"));
}


/**************************************** METHODS ***************************************/

Handle<Value> PropertiesList_ToArray(const Arguments& args)
{
    HandleScope handle_scope;

    PropertiesList* self = GetPtr(args.This());
    assert(self);

    Handle<Array> jsCategories = Array::New();

    PropertiesList::tCategoriesIterator catIter = self->getCategoriesIterator();
    while (catIter.hasMoreElements())
    {
        PropertiesList::tCategory* category = catIter.peekNextPtr();

        Handle<Value> jsProperties = toJS(
                    PropertiesList::tPropertiesIterator(category->values.begin(),
                                                         category->values.end()));

        Handle<Object> jsCategory = Object::New();
        jsCategory->Set(String::New("name"), String::New(category->strName.c_str()));
        jsCategory->Set(String::New("properties"), jsProperties);

        jsCategories->Set(jsCategories->Length(), jsCategory);

        catIter.moveNext();
    }

    return handle_scope.Close(jsCategories);
}

//-----------------------------------------------------------------------

Handle<Value> PropertiesList_SelectCategory(const Arguments& args)
{
    HandleScope handle_scope;

    PropertiesList* self = GetPtr(args.This());
    assert(self);

    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsBoolean())
    {
        self->selectCategory(*String::AsciiValue(args[0]->ToString()), args[1]->ToBoolean()->Value());
    }
    else if ((args.Length() == 1) && args[0]->IsString())
    {
        self->selectCategory(*String::AsciiValue(args[0]->ToString()));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nselectCategory(category, insertAtEnd)\nselectCategory(property)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> PropertiesList_Set(const Arguments& args)
{
    HandleScope handle_scope;

    PropertiesList* self = GetPtr(args.This());
    assert(self);

    if ((args.Length() == 3) && args[0]->IsString() && args[1]->IsString())
    {
        self->set(*String::AsciiValue(args[0]->ToString()),
                  *String::AsciiValue(args[1]->ToString()),
                  fromJS(args[2]));
    }
    else if ((args.Length() == 2) && args[0]->IsString())
    {
        self->set(*String::AsciiValue(args[0]->ToString()), fromJS(args[1]));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nset(category, property, value)\nset(property, value)"));
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> PropertiesList_Get(const Arguments& args)
{
    HandleScope handle_scope;

    PropertiesList* self = GetPtr(args.This());
    assert(self);

    Variant* pValue = 0;

    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsString())
    {
        pValue = self->get(*String::AsciiValue(args[0]->ToString()),
                           *String::AsciiValue(args[1]->ToString()));
    }
    else if ((args.Length() == 1) && args[0]->IsString())
    {
        pValue = self->get(*String::AsciiValue(args[0]->ToString()));
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nget(category, property)\nget(property)"));
    }

    if (!pValue)
        return Handle<Value>();

    return handle_scope.Close(toJS(pValue));
}

//-----------------------------------------------------------------------

Handle<Value> PropertiesList_GetProperties(const Arguments& args)
{
    HandleScope handle_scope;

    PropertiesList* self = GetPtr(args.This());
    assert(self);

    Handle<Value> jsProperties;

    if ((args.Length() == 1) && args[0]->IsString())
    {
        jsProperties = toJS(self->getPropertiesIterator(*String::AsciiValue(args[0]->ToString())));
    }
    else if (args.Length() == 0)
    {
        jsProperties = toJS(self->getPropertiesIterator());
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\ngetProperties(category)\ngetProperties()"));
    }

    return handle_scope.Close(jsProperties);
}

//-----------------------------------------------------------------------

Handle<Value> PropertiesList_Append(const Arguments& args)
{
    HandleScope handle_scope;

    PropertiesList* self = GetPtr(args.This());
    assert(self);

    PropertiesList* list2 = 0;

    if ((args.Length() >= 1) && args[0]->IsObject())
        list2 = GetPtr(args[0]);

    if (list2 && (args.Length() == 2) && args[1]->IsBoolean())
    {
        self->append(list2, args[1]->ToBoolean()->Value());
    }
    else if (list2 && args.Length() == 1)
    {
        self->append(list2);
    }
    else
    {
        return ThrowException(String::New("Invalid parameters, valid syntax:\nappend(list, atEnd)\nappend(list)"));
    }

    return Handle<Value>();
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Utils_PropertiesList(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    Handle<FunctionTemplate> propertiesList = pManager->getClassTemplate("Athena.Utils.PropertiesList");

    if (propertiesList.IsEmpty())
    {
        // Declaration of the class
        propertiesList = FunctionTemplate::New(PropertiesList_New);
        propertiesList->InstanceTemplate()->SetInternalFieldCount(1);

        // Methods
        AddMethod(propertiesList, "toArray",        PropertiesList_ToArray);
        AddMethod(propertiesList, "selectCategory", PropertiesList_SelectCategory);
        AddMethod(propertiesList, "set",            PropertiesList_Set);
        AddMethod(propertiesList, "get",            PropertiesList_Get);
        AddMethod(propertiesList, "getProperties",  PropertiesList_GetProperties);
        AddMethod(propertiesList, "append",         PropertiesList_Append);

        pManager->declareClassTemplate("Athena.Utils.PropertiesList", propertiesList);
    }

    // Add the class to the parent
    return parent->Set(String::New("PropertiesList"), propertiesList->GetFunction());
}
