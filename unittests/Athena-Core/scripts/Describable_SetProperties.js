desc = new Tests.MockDescribable2();

properties = new Athena.Utils.PropertiesList();

properties.selectCategory("Cat1");
properties.set("name", "hello");

properties.selectCategory("Cat2", false);
properties.set("index", 200);


delayedProperties = desc.setProperties(properties);
CHECK(delayedProperties === undefined, "delayedProperties === undefined");


CHECK(desc.unknownProperties() === undefined, "desc.unknownProperties() === undefined");

CHECK_EQUAL("hello", desc.name);
CHECK_EQUAL(200, desc.index);
