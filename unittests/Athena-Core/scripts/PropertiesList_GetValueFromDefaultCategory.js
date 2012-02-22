list = new Athena.Utils.PropertiesList();

list.set("string", "test");

CHECK_EQUAL("test", list.get("DEFAULT", "string"));
