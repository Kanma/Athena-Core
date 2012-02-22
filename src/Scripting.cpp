/** @file   Scripting.cpp
    @author Philip Abbet

    Implementation of the scripting-related functions of the Athena-Core module
*/

#include <Athena-Core/Scripting.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Color.h>
#include <Athena-Math/Scripting.h>

using namespace Athena::Utils;
using namespace Athena::Math;
using namespace v8;


Handle<Value> Athena::Utils::toJS(Variant* pValue)
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

Handle<Value> Athena::Utils::toJS(PropertiesList::tPropertiesIterator propIter)
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

Variant* Athena::Utils::fromJS(Handle<Value> value)
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
