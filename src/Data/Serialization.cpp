/** @file   Serialization.cpp
    @author Philip Abbet

    Implementation of the serialization / deserialization functions
*/

#include <Athena-Core/Data/Serialization.h>
#include <Athena-Core/Utils/Describable.h>
#include <Athena-Core/Utils/PropertiesList.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Quaternion.h>
#include <Athena-Math/Color.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


using namespace Athena::Data;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace rapidjson;


void Athena::Data::toJSON(Utils::Variant* pVariant, rapidjson::Value &value,
                          Value::AllocatorType &allocator)
{
    if (pVariant->isNull())
    {
        value.SetNull();
        return;
    }

    switch (pVariant->getType())
    {
        case Variant::STRING:
            value.SetString(pVariant->toString().c_str());
            break;

        case Variant::INTEGER:
        case Variant::SHORT:
        case Variant::CHAR:
            value.SetInt(pVariant->toInt());
            break;

        case Variant::UNSIGNED_INTEGER:
        case Variant::UNSIGNED_SHORT:
        case Variant::UNSIGNED_CHAR:
            value.SetUint(pVariant->toUInt());
            break;

        case Variant::FLOAT:
        case Variant::DOUBLE:
            value.SetDouble(pVariant->toDouble());
            break;

        case Variant::BOOLEAN:
            value.SetBool(pVariant->toBool());
            break;

        case Variant::VECTOR3:
        {
            Value field;
            Vector3 vec = pVariant->toVector3();

            value.SetObject();

            field.SetDouble(vec.x);
            value.AddMember("x", field, allocator);

            field.SetDouble(vec.y);
            value.AddMember("y", field, allocator);

            field.SetDouble(vec.z);
            value.AddMember("z", field, allocator);
            break;
        }

        case Variant::QUATERNION:
        {
            Value field;
            Quaternion q = pVariant->toQuaternion();

            value.SetObject();

            field.SetDouble(q.x);
            value.AddMember("x", field, allocator);

            field.SetDouble(q.y);
            value.AddMember("y", field, allocator);

            field.SetDouble(q.z);
            value.AddMember("z", field, allocator);

            field.SetDouble(q.w);
            value.AddMember("w", field, allocator);
            break;
        }

        case Variant::COLOR:
        {
            Value field;
            Color color = pVariant->toColor();

            value.SetObject();

            field.SetDouble(color.r);
            value.AddMember("r", field, allocator);

            field.SetDouble(color.g);
            value.AddMember("g", field, allocator);

            field.SetDouble(color.b);
            value.AddMember("b", field, allocator);

            field.SetDouble(color.a);
            value.AddMember("a", field, allocator);
            break;
        }

        case Variant::RADIAN:
        case Variant::DEGREE:
            value.SetDouble(pVariant->toRadian().valueRadians());
            break;

        case Variant::STRUCT:
        {
            Value field;

            value.SetObject();

            Variant::tFieldsIterator iter = pVariant->getFieldsIterator();
            while (iter.hasMoreElements())
            {
                toJSON(iter.peekNextValue(), field, allocator);
                value.AddMember(iter.peekNextKey().c_str(), field, allocator);
                iter.moveNext();
            }

            break;
        }
    }
}

//-----------------------------------------------------------------------

std::string Athena::Data::toJSON(Utils::Describable* pDescribable)
{
    // Assertions
    assert(pDescribable);

    // Retrieve the properties of the describable
    PropertiesList* pProperties = pDescribable->getProperties();

    PropertiesList* pUnknownProperties = pDescribable->getUnknownProperties();
    if (pUnknownProperties)
        pProperties->append(pUnknownProperties, false);

    // Create the JSON representation
    Document document;
    document.SetArray();

    PropertiesList::tCategoriesIterator categIter = pProperties->getCategoriesIterator();
    Value value;
    while (categIter.hasMoreElements())
    {
        PropertiesList::tCategory* pCategory = categIter.peekNextPtr();

        Value category;
        category.SetObject();

        value.SetString(pCategory->strName.c_str());
        category.AddMember("__category__", value, document.GetAllocator());

        PropertiesList::tPropertiesIterator propIter(pCategory->values.begin(), pCategory->values.end());
        while (propIter.hasMoreElements())
        {
            PropertiesList::tProperty* pProperty = propIter.peekNextPtr();

            toJSON(pProperty->pValue, value, document.GetAllocator());
            category.AddMember(pProperty->strName.c_str(), value, document.GetAllocator());

            propIter.moveNext();
        }

        document.PushBack(category, document.GetAllocator());

        categIter.moveNext();
    }

    // Convert it to string
    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);
    document.Accept(writer);

    delete pProperties;

    return s.GetString();
}
