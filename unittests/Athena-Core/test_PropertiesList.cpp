#include <UnitTest++.h>
#include <Athena-Core/PropertiesList.h>
#include <Athena-Core/Variant.h>

using namespace Athena;
using namespace Athena::Utils;


SUITE(PropertiesListTests)
{
    TEST(SetGetValueWithCategory)
    {
    	PropertiesList list;
    	Variant* pValue = new Variant("test");

    	list.set("TestCat", "string", pValue);

    	Variant* pTest = list.get("TestCat", "string");
    	CHECK(pTest);
    	CHECK_EQUAL(pValue->toString(), pTest->toString());
    }


    TEST(SetGetValueWithoutCategory)
    {
    	PropertiesList list;
    	Variant* pValue = new Variant("test");

    	list.set("string", pValue);

    	Variant* pTest = list.get("string");
    	CHECK(pTest);
    	CHECK_EQUAL(pValue->toString(), pTest->toString());
    }


    TEST(GetValueFromDefaultCategory)
    {
    	PropertiesList list;
    	Variant* pValue = new Variant("test");

    	list.set("string", pValue);

    	Variant* pTest = list.get("DEFAULT", "string");
    	CHECK(pTest);
    	CHECK_EQUAL(pValue->toString(), pTest->toString());
    }


    TEST(GetValueFromSelectedCategory)
    {
    	PropertiesList list;
    	Variant* pValue = new Variant("test");

    	list.set("TestCat", "string", pValue);
    	list.selectCategory("TestCat");

    	Variant* pTest = list.get("string");
    	CHECK(pTest);
    	CHECK_EQUAL(pValue->toString(), pTest->toString());
    }


    TEST(ModifyValue)
    {
    	PropertiesList list;
    	Variant* pValue = new Variant("test");

    	list.set("TestCat", "string", pValue);

    	pValue = new Variant("test2");
	
    	list.set("TestCat", "string", pValue);

    	Variant* pTest = list.get("string");
    	CHECK(pTest);
    	CHECK_EQUAL(pValue->toString(), pTest->toString());
    }


    TEST(SelectCategories)
    {
    	PropertiesList list;

    	list.selectCategory("Cat1");
    	list.selectCategory("Cat2");
    	list.selectCategory("Cat3");

    	PropertiesList::tCategoriesIterator iter = list.getCategoriesIterator();

    	CHECK(iter.hasMoreElements());
    	PropertiesList::tCategory category = iter.getNext();
    	CHECK_EQUAL("Cat1", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat2", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat3", category.strName);

    	CHECK(!iter.hasMoreElements());
    }


    TEST(InsertCategoryAtBeginning)
    {
    	PropertiesList list;

    	list.selectCategory("Cat1");
    	list.selectCategory("Cat2");
    	list.selectCategory("Cat3");

    	list.selectCategory("Cat4", false);

    	PropertiesList::tCategoriesIterator iter = list.getCategoriesIterator();

    	CHECK(iter.hasMoreElements());
    	PropertiesList::tCategory category = iter.getNext();
    	CHECK_EQUAL("Cat4", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat1", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat2", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat3", category.strName);

    	CHECK(!iter.hasMoreElements());
    }



    TEST(ValuesEnumeration)
    {
    	PropertiesList list;

    	list.selectCategory("Cat");

    	list.set("string", new Variant("test"));
    	list.set("int", new Variant(10));
    	list.set("bool", new Variant(true));

    	PropertiesList::tPropertiesIterator iter = list.getPropertiesIterator("Cat");

    	CHECK(iter.hasMoreElements());

    	while (iter.hasMoreElements())
    	{
    		PropertiesList::tProperty prop = iter.getNext();

    		if (prop.strName == "string")
    		{
    			Variant* pTest = prop.pValue;
                CHECK(pTest);
    			CHECK_EQUAL("test", pTest->toString());
    		}
    		else if (prop.strName == "int")
    		{
    			Variant* pTest = prop.pValue;
                CHECK(pTest);
    			CHECK_EQUAL(10, pTest->toInt());
    		}
    		else if (prop.strName == "bool")
    		{
    			Variant* pTest = prop.pValue;
                CHECK(pTest);
    			CHECK_EQUAL(true, pTest->toBool());
    		}
    		else
    		{
    			CHECK(false);
    		}
    	}
    }



    TEST(ValuesEnumerationInSelectedCategory)
    {
    	PropertiesList list;

    	list.selectCategory("Cat");

    	list.set("string", new Variant("test"));
    	list.set("int", new Variant(10));
    	list.set("bool", new Variant(true));

    	list.selectCategory("Empty");
    	list.selectCategory("Cat");


    	PropertiesList::tPropertiesIterator iter = list.getPropertiesIterator();

    	CHECK(iter.hasMoreElements());

    	while (iter.hasMoreElements())
    	{
    		PropertiesList::tProperty prop = iter.getNext();

    		if (prop.strName == "string")
    		{
    			Variant* pTest = prop.pValue;
                CHECK(pTest);
    			CHECK_EQUAL("test", pTest->toString());
    		}
    		else if (prop.strName == "int")
    		{
    			Variant* pTest = prop.pValue;
                CHECK(pTest);
    			CHECK_EQUAL(10, pTest->toInt());
    		}
    		else if (prop.strName == "bool")
    		{
    			Variant* pTest = prop.pValue;
                CHECK(pTest);
    			CHECK_EQUAL(true, pTest->toBool());
    		}
    		else
    		{
    			CHECK(false);
    		}
    	}
    }


    TEST(ValuesEnumerationInNonExistingCategory)
    {
    	PropertiesList list;

    	PropertiesList::tPropertiesIterator iter = list.getPropertiesIterator("Unknown");

    	CHECK(!iter.hasMoreElements());
    }



    TEST(AppendList)
    {
    	PropertiesList list1;

    	list1.selectCategory("Cat1");
    	list1.set("string", new Variant("test"));
    	list1.set("int", new Variant(10));
    	list1.set("bool", new Variant(true));


    	PropertiesList list2;

    	list2.selectCategory("Cat2");
    	list2.set("float", new Variant(10.0f));

    	list1.append(&list2);


    	PropertiesList::tCategoriesIterator iter = list1.getCategoriesIterator();

    	CHECK(iter.hasMoreElements());
    	PropertiesList::tCategory category = iter.getNext();
    	CHECK_EQUAL("Cat1", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat2", category.strName);
    }


    TEST(PrependList)
    {
    	PropertiesList list1;

    	list1.selectCategory("Cat1");
    	list1.set("string", new Variant("test"));
    	list1.set("int", new Variant(10));
    	list1.set("bool", new Variant(true));


    	PropertiesList list2;

    	list2.selectCategory("Cat2");
    	list2.set("float", new Variant(10.0f));

    	list1.append(&list2, false);


    	PropertiesList::tCategoriesIterator iter = list1.getCategoriesIterator();

    	CHECK(iter.hasMoreElements());
    	PropertiesList::tCategory category = iter.getNext();
    	CHECK_EQUAL("Cat2", category.strName);

    	CHECK(iter.hasMoreElements());
    	category = iter.getNext();
    	CHECK_EQUAL("Cat1", category.strName);
    }
}
