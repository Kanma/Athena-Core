list = new Athena.Utils.PropertiesList();

list.selectCategory("Cat1");
list.selectCategory("Cat2");
list.selectCategory("Cat3");


categories = list.toArray();

CHECK_EQUAL(3, categories.length);

CHECK_EQUAL("Cat1", categories[0].name);
CHECK_EQUAL(0, categories[0].properties.length);

CHECK_EQUAL("Cat2", categories[1].name);
CHECK_EQUAL(0, categories[1].properties.length);

CHECK_EQUAL("Cat3", categories[2].name);
CHECK_EQUAL(0, categories[2].properties.length);
