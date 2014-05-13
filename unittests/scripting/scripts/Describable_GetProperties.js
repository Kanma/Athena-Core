desc = new Tests.MockDescribable2();

CHECK(desc.properties !== undefined, 'desc.properties !== undefined');

CHECK_EQUAL(desc.index, desc.properties.get("Cat2", "index"));
CHECK_EQUAL(desc.name, desc.properties.get("Cat1", "name"));
