desc = new Tests.MockDescribable2();

properties = desc.properties();
CHECK(properties !== undefined, 'properties !== undefined');

categories = properties.toArray();

CHECK_EQUAL(2, categories.length);

CHECK_EQUAL("Cat2", categories[0].name);
CHECK_EQUAL(1, categories[0].properties.length);
CHECK_EQUAL("index", categories[0].properties[0].name);
CHECK_EQUAL(desc.index, categories[0].properties[0].value);

CHECK_EQUAL("Cat1", categories[1].name);
CHECK_EQUAL(1, categories[1].properties.length);
CHECK_EQUAL("name", categories[1].properties[0].name);
CHECK_EQUAL(desc.name, categories[1].properties[0].value);
