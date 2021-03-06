/** @file   Serialization.cpp
    @author Philip Abbet

    Implementation of the serialization / deserialization functions
*/

#include <Athena-Core/Data/Serialization.h>
#include <Athena-Core/Data/FileDataStream.h>
#include <Athena-Core/Utils/Describable.h>
#include <Athena-Core/Utils/PropertiesList.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Quaternion.h>
#include <Athena-Math/Color.h>
#include <Athena-Core/Log/LogManager.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


using namespace Athena::Data;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace Athena::Log;
using namespace rapidjson;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Serialization";


/************************************** FUNCTIONS ***************************************/

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
            value.SetString(pVariant->toString().c_str(), allocator);
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
            Value name;
            Value field;

            value.SetObject();

            Variant::tFieldsIterator iter = pVariant->getFieldsIterator();
            while (iter.hasMoreElements())
            {
                toJSON(iter.peekNextValue(), field, allocator);
                name.SetString(iter.peekNextKey().c_str(), allocator);
                value.AddMember(name, field, allocator);
                iter.moveNext();
            }

            break;
        }

        case Variant::NONE:
            // Should not happen, but the compiler complain if not present
            break;
    }
}

//-----------------------------------------------------------------------

void Athena::Data::fromJSON(const rapidjson::Value& value, Utils::Variant* pVariant)
{
    // Assertions
    assert(pVariant);

    switch (value.GetType())
    {
        case rapidjson::kNullType:
            *pVariant = Variant();
            break;

        case rapidjson::kFalseType:
        case rapidjson::kTrueType:
            *pVariant = Variant(value.GetBool());
            break;

        case rapidjson::kStringType:
            *pVariant = Variant(value.GetString());
            break;

        case rapidjson::kNumberType:
            if (value.IsUint())
                *pVariant = Variant(value.GetUint());
            else if (value.IsInt())
                *pVariant = Variant(value.GetInt());
            else if (value.IsDouble())
                *pVariant = Variant(value.GetDouble());
            break;

        case rapidjson::kObjectType:
        {
            if (value.HasMember("x") && value.HasMember("y") && value.HasMember("z"))
            {
                if (value.HasMember("w"))
                {
                    *pVariant = Variant(Quaternion(value["w"].GetDouble(),
                                                   value["x"].GetDouble(),
                                                   value["y"].GetDouble(),
                                                   value["z"].GetDouble()));
                }
                else
                {
                    *pVariant = Variant(Vector3(value["x"].GetDouble(),
                                                value["y"].GetDouble(),
                                                value["z"].GetDouble()));
                }
            }
            else if (value.HasMember("r") && value.HasMember("g") &&
                     value.HasMember("b") && value.HasMember("a"))
            {
                *pVariant = Variant(Color(value["r"].GetDouble(),
                                          value["g"].GetDouble(),
                                          value["b"].GetDouble(),
                                          value["a"].GetDouble()));
            }
            else
            {
                *pVariant = Variant(Variant::STRUCT);

                Value::ConstMemberIterator iter, iterEnd;
                for (iter = value.MemberBegin(), iterEnd = value.MemberEnd();
                     iter != iterEnd; ++iter)
                {
                    Variant* pField = new Variant();
                    fromJSON(iter->value, pField);
                    pVariant->setField(iter->name.GetString(), pField);
                }
            }

            break;
        }

        case rapidjson::kArrayType:
            // Should not happen, but the compiler complain if not present
            break;
    }
}

//-----------------------------------------------------------------------

void Athena::Data::toJSON(Utils::Describable* pDescribable, rapidjson::Value &json_describable,
                          rapidjson::Value::AllocatorType &allocator)
{
    // Assertions
    assert(pDescribable);

    // Retrieve the properties of the describable
    PropertiesList* pProperties = pDescribable->getProperties();

    PropertiesList* pUnknownProperties = pDescribable->getUnknownProperties();
    if (pUnknownProperties)
        pProperties->append(pUnknownProperties, false);

    // Create the JSON representation
    json_describable.SetArray();

    PropertiesList::tCategoriesIterator categIter = pProperties->getCategoriesIterator();
    Value name;
    Value value;
    while (categIter.hasMoreElements())
    {
        PropertiesList::tCategory* pCategory = categIter.peekNextPtr();

        Value category;
        category.SetObject();

        value.SetString(pCategory->strName.c_str(), allocator);
        category.AddMember("__category__", value, allocator);

        PropertiesList::tPropertiesIterator propIter(pCategory->values.begin(),
                                                     pCategory->values.end());
        while (propIter.hasMoreElements())
        {
            PropertiesList::tProperty* pProperty = propIter.peekNextPtr();

            toJSON(pProperty->pValue, value, allocator);

            name.SetString(pProperty->strName.c_str(), allocator);
            category.AddMember(name, value, allocator);

            propIter.moveNext();
        }

        json_describable.PushBack(category, allocator);

        categIter.moveNext();
    }

    delete pProperties;
}

//-----------------------------------------------------------------------

void Athena::Data::fromJSON(const rapidjson::Value& json_describable,
                            Utils::Describable* pDescribable,
                            PropertiesList* pDelayedProperties)
{
    // Assertions
    assert(pDescribable);

    if (!json_describable.IsArray())
        return;

    // Create the properties of the describable
    PropertiesList* pProperties = new PropertiesList();

    Value::ConstValueIterator iter, iterEnd;
    for (iter = json_describable.Begin(), iterEnd = json_describable.End();
         iter != iterEnd; ++iter)
    {
        pProperties->selectCategory((*iter)["__category__"].GetString());

        Value::ConstMemberIterator iter2, iterEnd2;
        for (iter2 = iter->MemberBegin(), iterEnd2 = iter->MemberEnd();
             iter2 != iterEnd2; ++iter2)
        {
            if (iter2->name.GetString() == std::string("__category__"))
                continue;

            Variant* pField = new Variant();
            fromJSON(iter2->value, pField);
            pProperties->set(iter2->name.GetString(), pField);
        }
    }

    // Assign the properties to the describable
    pDescribable->setProperties(pProperties, pDelayedProperties);

    delete pProperties;
}

//-----------------------------------------------------------------------

std::string Athena::Data::toJSON(Utils::Describable* pDescribable)
{
    // Assertions
    assert(pDescribable);

    // Retrieve the JSON representation
    Document document;
    toJSON(pDescribable, document, document.GetAllocator());

    // Convert it to string
    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);
    document.Accept(writer);

    return s.GetString();
}

//-----------------------------------------------------------------------

bool Athena::Data::fromJSON(const std::string& json_describable,
                            Utils::Describable* pDescribable,
                            PropertiesList* pDelayedProperties)
{
    // Assertions
    assert(pDescribable);

    // Convert to a JSON representation
    Document document;
	if (document.Parse<0>(json_describable.c_str()).HasParseError())
    {
        ATHENA_LOG_ERROR(document.GetParseError());
        return false;
    }

    fromJSON(document, pDescribable, pDelayedProperties);

    return true;
}

//-----------------------------------------------------------------------

bool Athena::Data::loadJSONFile(const std::string& strFileName,
                                rapidjson::Document &document)
{
    // Assertions
    assert(!strFileName.empty());

    // Read the content of the file
    FileDataStream stream(strFileName, DataStream::READ);
    if (!stream.isOpen())
    {
        ATHENA_LOG_ERROR("File not found: " + strFileName);
        return false;
    }

    std::string content;

    char buffer[1025];

    while (!stream.eof())
    {
        size_t count = stream.read(buffer, 1024);
        buffer[count] = 0;
        content += buffer;
    }

    stream.close();

    // Convert to a JSON representation
	if (document.Parse<0>(content.c_str()).HasParseError())
    {
        ATHENA_LOG_ERROR(document.GetParseError());
        return false;
    }

    return true;
}
