desc = new Tests.MockDescribable2();

properties = desc.properties();
CHECK(properties !== undefined, 'properties !== undefined');

CHECK_EQUAL(desc.index, properties.get("Cat2", "index"));
CHECK_EQUAL(desc.name, properties.get("Cat1", "name"));
