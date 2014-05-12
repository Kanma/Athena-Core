list = new Athena.Utils.PropertiesList();

list.set("TestCat", "string", "test");
list.set("TestCat", "string", "test2");

CHECK_EQUAL("test2", list.get("string"));
