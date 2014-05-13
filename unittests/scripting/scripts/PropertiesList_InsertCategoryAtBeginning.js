list = new Athena.Utils.PropertiesList();

list.selectCategory("Cat1");
list.selectCategory("Cat2");
list.selectCategory("Cat3");

list.selectCategory("Cat4", false);


categories = list.toArray();

CHECK_EQUAL(4, categories.length);

CHECK_EQUAL("Cat4", categories[0].name);
CHECK_EQUAL(0, categories[0].properties.length);

CHECK_EQUAL("Cat1", categories[1].name);
CHECK_EQUAL(0, categories[1].properties.length);

CHECK_EQUAL("Cat2", categories[2].name);
CHECK_EQUAL(0, categories[2].properties.length);

CHECK_EQUAL("Cat3", categories[3].name);
CHECK_EQUAL(0, categories[3].properties.length);
