list = new Athena.Utils.PropertiesList();

list.selectCategory("Cat");

list.set("string", "test");
list.set("int", 10);
list.set("bool", true);

list.selectCategory("Empty");
list.selectCategory("Cat");


properties = list.getProperties("Cat");

CHECK_EQUAL(3, properties.length);

CHECK_EQUAL("string", properties[0].name);
CHECK_EQUAL("test", properties[0].value);

CHECK_EQUAL("int", properties[1].name);
CHECK_EQUAL(10, properties[1].value);

CHECK_EQUAL("bool", properties[2].name);
CHECK_EQUAL(true, properties[2].value);
