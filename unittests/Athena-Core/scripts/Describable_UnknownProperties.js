desc = new Tests.MockDescribable2();

properties = new Athena.Utils.PropertiesList();

properties.selectCategory("Cat3", false);
properties.set("value", 37);

desc.setProperties(properties);


unknownProperties = desc.unknownProperties();
CHECK(unknownProperties !== undefined, "unknownProperties !== undefined");

CHECK_EQUAL(37, unknownProperties.get("Cat3", "value"));
