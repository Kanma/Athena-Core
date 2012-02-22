list1 = new Athena.Utils.PropertiesList();

list1.selectCategory("Cat1");

list1.set("string", "test");
list1.set("int", 10);
list1.set("bool", true);


list2 = new Athena.Utils.PropertiesList();

list2.selectCategory("Cat2");

list2.set("float", 10.0);


list1.append(list2);


categories = list1.toArray();

CHECK_EQUAL(2, categories.length);

CHECK_EQUAL("Cat1", categories[0].name);
CHECK_EQUAL(3, categories[0].properties.length);

CHECK_EQUAL("Cat2", categories[1].name);
CHECK_EQUAL(1, categories[1].properties.length);
