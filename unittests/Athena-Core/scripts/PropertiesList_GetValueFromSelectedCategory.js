list = new Athena.Utils.PropertiesList();

list.set("TestCat", "string", "test");
list.selectCategory("TestCat");

CHECK_EQUAL("test", list.get("string"));
