list = new Athena.Utils.PropertiesList();

properties = list.getProperties("Unknown");

CHECK_EQUAL(0, properties.length);
