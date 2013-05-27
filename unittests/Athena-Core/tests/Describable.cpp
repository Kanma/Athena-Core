#include <UnitTest++.h>
#include <Athena-Core/Utils/Describable.h>
#include <Athena-Core/Data/Serialization.h>
#include "../mocks/Describable.h"

using namespace Athena;
using namespace Athena::Utils;
using namespace Athena::Data;


SUITE(DescribableTests)
{
    TEST(CategoriesOrder)
    {
        MockDescribable2 desc;

        PropertiesList* pProperties = desc.getProperties();
        CHECK(pProperties);

        PropertiesList::tCategoriesIterator iter = pProperties->getCategoriesIterator();

        CHECK(iter.hasMoreElements());

        PropertiesList::tCategory category = iter.getNext();
        CHECK_EQUAL("Cat2", category.strName);

        CHECK(iter.hasMoreElements());

        category = iter.getNext();
        CHECK_EQUAL("Cat1", category.strName);

        CHECK(!iter.hasMoreElements());

        delete pProperties;
    }


    TEST(GetProperties)
    {
        MockDescribable2 desc;

        PropertiesList* pProperties = desc.getProperties();
        CHECK(pProperties);

        Variant* pStringValue = pProperties->get("Cat1", "name");
        Variant* pIntValue = pProperties->get("Cat2", "index");

        CHECK(pStringValue);
        CHECK(pIntValue);
        CHECK_EQUAL(desc.strName, pStringValue->toString());
        CHECK_EQUAL(desc.iIndex, pIntValue->toInt());

        delete pProperties;
    }


    TEST(SetProperties)
    {
        MockDescribable2 desc;

        PropertiesList* pProperties = new PropertiesList();

        pProperties->selectCategory("Cat1");
        pProperties->set("name", new Variant("hello"));

        pProperties->selectCategory("Cat2", false);
        pProperties->set("index", new Variant(200));

        desc.setProperties(pProperties);
        delete pProperties;

        CHECK(!desc.getUnknownProperties());
        CHECK_EQUAL("hello", desc.strName);
        CHECK_EQUAL(200, desc.iIndex);
    }


    TEST(UnknownProperties)
    {
        MockDescribable2 desc;

        PropertiesList* pProperties = new PropertiesList();

        pProperties->selectCategory("Cat3", false);
        pProperties->set("value", new Variant(37));

        desc.setProperties(pProperties);
        delete pProperties;


        PropertiesList* pUnknowProperties = desc.getUnknownProperties();
        CHECK(pUnknowProperties);

        Variant* pIntValue = pUnknowProperties->get("Cat3", "value");
        CHECK(pIntValue);
        CHECK_EQUAL(37, pIntValue->toInt());
    }


    TEST(DelayedProperties)
    {
        MockDescribable2 desc;

        PropertiesList* pProperties = new PropertiesList();

        pProperties->selectCategory("Cat1");
        pProperties->set("name", new Variant("hello"));
        pProperties->set("delayed", new Variant("something"));

        pProperties->selectCategory("Cat2", false);
        pProperties->set("index", new Variant(200));


        PropertiesList* pDelayedProperties = new PropertiesList();

        desc.setProperties(pProperties, pDelayedProperties);

        delete pProperties;


        PropertiesList::tCategoriesIterator catIter = pDelayedProperties->getCategoriesIterator();

        CHECK(catIter.hasMoreElements());

        PropertiesList::tCategory category = catIter.getNext();
        CHECK_EQUAL("Cat1", category.strName);

        CHECK_EQUAL(1, (unsigned int) category.values.size());
        CHECK_EQUAL("delayed", category.values[0].strName);
        CHECK_EQUAL("something", category.values[0].pValue->toString());

        CHECK(!catIter.hasMoreElements());

        delete pDelayedProperties;
    }
}


SUITE(DescribableJSONSerialization)
{
    TEST(SerializationToObject)
    {
        MockDescribable2 desc;

        rapidjson::Document document;

        toJSON(&desc, document, document.GetAllocator());

        CHECK(document.IsArray());
        CHECK_EQUAL(2, document.Size());

        CHECK(document[(rapidjson::SizeType) 0].IsObject());
        CHECK_EQUAL("Cat2", document[(rapidjson::SizeType) 0]["__category__"].GetString());
        CHECK_EQUAL(10, document[(rapidjson::SizeType) 0]["index"].GetInt());

        CHECK(document[1].IsObject());
        CHECK_EQUAL("Cat1", document[1]["__category__"].GetString());
        CHECK_EQUAL("test", document[1]["name"].GetString());
    }

    TEST(SerializationToString)
    {
        MockDescribable2 desc;

        std::string json = toJSON(&desc);

        CHECK_EQUAL("[\n    {\n        \"__category__\": \"Cat2\",\n        \"index\": 10\n    },\n    {\n        \"__category__\": \"Cat1\",\n        \"name\": \"test\"\n    }\n]", json);
    }
}


SUITE(DescribableJSONDeserialization)
{
    TEST(DeserializationFromObject)
    {
        MockDescribable2 desc;

        rapidjson::Document document;
        document.SetArray();

        rapidjson::Value entry;
        rapidjson::Value field;

        entry.SetObject();

        field.SetString("Cat2");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetUint(100);
        entry.AddMember("index", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());

        entry.SetObject();

        field.SetString("Cat1");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetString("test2");
        entry.AddMember("name", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());


        fromJSON(document, &desc);

        CHECK_EQUAL("test2", desc.strName);
        CHECK_EQUAL(100, desc.iIndex);

        CHECK(!desc.getUnknownProperties());
    }


    TEST(DeserializationFromObjectWithUnknownProperties)
    {
        MockDescribable2 desc;

        rapidjson::Document document;
        document.SetArray();

        rapidjson::Value entry;
        rapidjson::Value field;

        entry.SetObject();

        field.SetString("Cat3");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetUint(20);
        entry.AddMember("unknown", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());

        entry.SetObject();

        field.SetString("Cat2");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetUint(100);
        entry.AddMember("index", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());

        entry.SetObject();

        field.SetString("Cat1");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetString("test2");
        entry.AddMember("name", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());


        fromJSON(document, &desc);

        CHECK_EQUAL("test2", desc.strName);
        CHECK_EQUAL(100, desc.iIndex);


        PropertiesList* pUnknownProperties = desc.getUnknownProperties();
        CHECK(pUnknownProperties);

        Variant* pUnknownValue = pUnknownProperties->get("Cat3", "unknown");

        CHECK(pUnknownValue);
        CHECK_EQUAL(20, pUnknownValue->toInt());
    }


    TEST(DeserializationFromObjectWithDelayedProperties)
    {
        MockDescribable2 desc;

        rapidjson::Document document;
        document.SetArray();

        rapidjson::Value entry;
        rapidjson::Value field;

        entry.SetObject();

        field.SetString("Cat2");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetUint(100);
        entry.AddMember("index", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());

        entry.SetObject();

        field.SetString("Cat1");
        entry.AddMember("__category__", field, document.GetAllocator());

        field.SetString("test2");
        entry.AddMember("name", field, document.GetAllocator());

        field.SetString("unused");
        entry.AddMember("delayed", field, document.GetAllocator());

        document.PushBack(entry, document.GetAllocator());

        PropertiesList* pDelayedProperties = new PropertiesList();

        fromJSON(document, &desc, pDelayedProperties);

        CHECK_EQUAL("test2", desc.strName);
        CHECK_EQUAL(100, desc.iIndex);

        Variant* pDelayedValue = pDelayedProperties->get("Cat1", "delayed");

        CHECK(pDelayedValue);
        CHECK_EQUAL("unused", pDelayedValue->toString());

        delete pDelayedProperties;
    }


    TEST(DeserializationFromString)
    {
        MockDescribable2 desc;

        CHECK(fromJSON(std::string("[\n    {\n        \"__category__\": \"Cat2\",\n        \"index\": 100\n    },\n    {\n        \"__category__\": \"Cat1\",\n        \"name\": \"test2\"\n    }\n]"), &desc));

        CHECK_EQUAL("test2", desc.strName);
        CHECK_EQUAL(100, desc.iIndex);

        CHECK(!desc.getUnknownProperties());
    }


    TEST(DeserializationFromStringWithUnknownProperties)
    {
        MockDescribable2 desc;

        CHECK(fromJSON(std::string("[\n    {\n        \"__category__\": \"Cat3\",\n        \"unknown\": 10\n    },\n    {\n        \"__category__\": \"Cat2\",\n        \"index\": 100\n    },\n    {\n        \"__category__\": \"Cat1\",\n        \"name\": \"test2\"\n    }\n]"), &desc));

        CHECK_EQUAL("test2", desc.strName);
        CHECK_EQUAL(100, desc.iIndex);

        PropertiesList* pUnknownProperties = desc.getUnknownProperties();
        CHECK(pUnknownProperties);

        Variant* pUnknownValue = pUnknownProperties->get("Cat3", "unknown");

        CHECK(pUnknownValue);
        CHECK_EQUAL(10, pUnknownValue->toInt());
    }


    TEST(DeserializationFromStringWithDelayedProperties)
    {
        MockDescribable2 desc;

        PropertiesList* pDelayedProperties = new PropertiesList();

        CHECK(fromJSON(std::string("[\n    {\n        \"__category__\": \"Cat2\",\n        \"index\": 100\n    },\n    {\n        \"__category__\": \"Cat1\",\n        \"name\": \"test2\",\n        \"delayed\": \"unused\"\n    }\n]"), &desc, pDelayedProperties));

        CHECK_EQUAL("test2", desc.strName);
        CHECK_EQUAL(100, desc.iIndex);

        Variant* pDelayedValue = pDelayedProperties->get("Cat1", "delayed");

        CHECK(pDelayedValue);
        CHECK_EQUAL("unused", pDelayedValue->toString());

        delete pDelayedProperties;
    }
}
