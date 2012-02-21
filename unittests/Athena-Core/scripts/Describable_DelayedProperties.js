desc = new Tests.MockDescribable2();

properties = new Athena.Utils.PropertiesList();

properties.selectCategory("Cat1");
properties.set("name", "hello");
properties.set("delayed", "something");

properties.selectCategory("Cat2", false);
properties.set("index", 200);


delayedProperties = desc.setProperties(properties);
CHECK(delayedProperties !== undefined, "delayedProperties !== undefined");


categories = delayedProperties.toArray();

CHECK_EQUAL(1, categories.length);

CHECK_EQUAL("Cat1", categories[0].name);
CHECK_EQUAL(1, categories[0].properties.length);
CHECK_EQUAL("delayed", categories[0].properties[0].name);
CHECK_EQUAL("something", categories[0].properties[0].value);
