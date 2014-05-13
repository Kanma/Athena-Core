list = new Athena.Utils.PropertiesList();

list.set("TestCat", "string", "test");

CHECK_EQUAL("test", list.get("TestCat", "string"));
