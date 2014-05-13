desc = new Tests.MockDescribable2();

properties = new Athena.Utils.PropertiesList();

properties.selectCategory("Cat3", false);
properties.set("value", 37);

desc.properties = properties;


CHECK(desc.unknownProperties !== undefined, "desc.unknownProperties !== undefined");

CHECK_EQUAL(37, desc.unknownProperties.get("Cat3", "value"));
