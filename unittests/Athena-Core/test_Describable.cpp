#include <UnitTest++.h>
#include <Athena-Core/Describable.h>
#include "mocks/Describable.h"

using namespace Athena;
using namespace Athena::Utils;


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
