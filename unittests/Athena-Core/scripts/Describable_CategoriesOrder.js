desc = new Tests.MockDescribable2();
//desc = new Athena.Utils.Describable();

properties = desc.properties();
CHECK(properties !== undefined, 'properties !== undefined');

for (var c in properties)
    print('  - ' + c + '\n');

// PropertiesList::tCategoriesIterator iter = pProperties->getCategoriesIterator();
// 
// CHECK(iter.hasMoreElements());
// 
// PropertiesList::tCategory category = iter.getNext();
// CHECK_EQUAL("Cat2", category.strName);
// 
// CHECK(iter.hasMoreElements());
// 
// category = iter.getNext();
// CHECK_EQUAL("Cat1", category.strName);
// 
// CHECK(!iter.hasMoreElements());
// 
// delete pProperties;
// 
// 
//  = new module1.Point3D(1, 2, 3);
// p.double()
// CHECK_CLOSE(2, p.x);
// CHECK_CLOSE(4, p.y);
// CHECK_CLOSE(6, p.z);
