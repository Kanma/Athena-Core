#include <UnitTest++.h>
#include <Athena-Core/Utils/StringsMap.h>

using namespace Athena;
using namespace Athena::Utils;


SUITE(StringsMapTests)
{
    TEST(RegisterStringWithID)
    {
    	const tID ID1 = 1;
    	const tID ID2 = 2;

    	StringsMap map;

    	CHECK(map.registerString(ID1, "1"));
    	CHECK(map.registerString(ID2, "2"));

    	CHECK_EQUAL("1", map.getString(ID1));
    	CHECK_EQUAL("2", map.getString(ID2));
    }


    TEST(RegisterStringWithExistingID)
    {
    	const tID ID1 = 1;
    	const tID ID2 = 2;

    	StringsMap map;

    	CHECK(map.registerString(ID1, "1"));
    	CHECK(!map.registerString(ID1, "2"));
    }



    TEST(RegisterStringWithoutID)
    {
    	StringsMap map;

    	tID ID1 = map.registerString("1");
    	CHECK(ID1 > 0);

    	tID ID2 = map.registerString("2");
    	CHECK(ID2 > 0);

    	CHECK(ID2 != ID1);

    	CHECK_EQUAL("1", map.getString(ID1));
    	CHECK_EQUAL("2", map.getString(ID2));
    }


    TEST(GetID)
    {
    	const tID ID1 = 1;
    	const tID ID2 = 2;

    	StringsMap map;

    	CHECK(map.registerString(ID1, "1"));
    	CHECK(map.registerString(ID2, "2"));

    	CHECK_EQUAL(ID1, map.getID("1"));
    	CHECK_EQUAL(ID2, map.getID("2"));
    }
}
