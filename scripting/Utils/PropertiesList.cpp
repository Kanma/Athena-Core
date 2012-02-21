/** @file   PropertiesList.cpp
    @author Philip Abbet
    
    Javascript binding of the class Athena::Utils::PropertiesList
*/

#include <Athena-Core/Utils/PropertiesList.h>
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

#define GetObjectPtr(HANDLE) GetObjectPtr<PropertiesList>(HANDLE)


/********************************** UTILITY FUNCTIONS **********************************/

Handle<Value> toJS(Variant* pValue)
{
    switch (pValue->getType())
    {
        case Variant::STRING:
            return String::New(pValue->toString().c_str());
        
        case Variant::INTEGER:
        case Variant::SHORT:
        case Variant::CHAR:
            return Int32::New(pValue->toInt());
        
        case Variant::UNSIGNED_INTEGER:
        case Variant::UNSIGNED_SHORT:
        case Variant::UNSIGNED_CHAR:
            return Uint32::New(pValue->toUInt());

        case Variant::FLOAT:
        case Variant::DOUBLE:
            return Number::New(pValue->toFloat());
        
        case Variant::BOOLEAN:
            return Boolean::New(pValue->toBool());

        case Variant::VECTOR3:
            return toJavaScript(pValue->toVector3());

        case Variant::QUATERNION:
            return toJavaScript(pValue->toQuaternion());

        case Variant::COLOR:
            return toJavaScript(pValue->toColor());

        case Variant::RADIAN:
        case Variant::DEGREE:
            return Number::New(pValue->toRadian().valueRadians());
        
        case Variant::STRUCT:
        {
            Handle<Object> obj = Object::New();
            
            Variant::tFieldsIterator iter = pValue->getFieldsIterator();
            while (iter.hasMoreElements())
            {
                obj->Set(String::New(iter.peekNextKey().c_str()), toJS(iter.peekNextValue()));
                iter.moveNext();
            }
            
            return obj;
        }
    }

    return Handle<Value>();
}

//-----------------------------------------------------------------------

Handle<Value> toJS(PropertiesList::tPropertiesIterator propIter)
{
    HandleScope handle_scope;

    Handle<Array> jsProperties = Array::New();
    
    while (propIter.hasMoreElements())
    {
        PropertiesList::tProperty* property = propIter.peekNextPtr();

        Handle<Object> jsProperty = Object::New();
        jsProperty->Set(String::New("name"), String::New(property->strName.c_str()));
        jsProperty->Set(String::New("value"), toJS(property->pValue));
        
        jsProperties->Set(jsProperties->Length(), jsProperty);
        
        propIter.moveNext();
    }

    return handle_scope.Close(jsProperties);
}

//-----------------------------------------------------------------------

Variant* fromJS(Handle<Value> value)
{
    if (value->IsString())
    {
        return new Variant(*String::AsciiValue(value->ToString()));
    }
    else if (value->IsUint32())
    {
        return new Variant(value->ToUint32()->Value());
    }
    else if (value->IsInt32())
    {
        return new Variant(value->ToInt32()->Value());
    }
    else if (value->IsNumber())
    {
        return new Variant(value->ToNumber()->Value());
    }
    else if (value->IsBoolean())
    {
        return new Variant(value->ToBoolean()->Value());
    }
    else if (value->IsObject())
    {
        Handle<Object> object = value->ToObject();
        Handle<Function> prototype = Handle<Function>::Cast(object->GetPrototype());
        
        if (prototype->Has(String::New("__classname__")))
        {
            std::string classname = *String::AsciiValue(prototype->Get(String::New("__classname__")));

            if (classname == "Athena.Math.Vector3")
                return new Variant(fromJSVector3(value));
            else if (classname == "Athena.Math.Quaternion")
                return new Variant(fromJSQuaternion(value));
            else if (classname == "Athena.Math.Color")
                return new Variant(fromJSColor(value));
        }
        else
        {
            Variant* pStruct = new Variant(Variant::STRUCT);

            Handle<Array> names = object->GetPropertyNames();
            for (unsigned int i = 0; i < names->Length(); ++i)
            {
                Handle<String> name = names->Get(i)->ToString();
                Variant* pValue = fromJS(object->Get(name));
                if (pValue)
                    pStruct->setField(*String::AsciiValue(name), pValue);
            }
            
            return pStruct;
        }
    }

    return 0;
}


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> PropertiesList_New(const Arguments& args)
{
    return SetObjectPtr(args.This(), new PropertiesList());
}


/**************************************** METHODS ***************************************/

Handle<Value> PropertiesList_ToArray(const Arguments& args)
{
    HandleScope handle_scope;
    
    PropertiesList* self = GetObjectPtr(args.This());
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

    PropertiesList* self = GetObjectPtr(args.This());
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

    PropertiesList* self = GetObjectPtr(args.This());
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

    PropertiesList* self = GetObjectPtr(args.This());
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
    
    PropertiesList* self = GetObjectPtr(args.This());
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
    
    PropertiesList* self = GetObjectPtr(args.This());
    assert(self);

    PropertiesList* list2 = 0;

    if ((args.Length() >= 1) && args[0]->IsObject())
        list2 = GetObjectPtr(args[0]);

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
