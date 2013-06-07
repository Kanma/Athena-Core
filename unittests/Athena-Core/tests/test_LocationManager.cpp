#include <UnitTest++.h>
#include <Athena-Core/Data/LocationManager.h>
#include <Athena-Core/Data/DataStream.h>

using namespace Athena;
using namespace Athena::Data;
using namespace std;


TEST(LocationManager_Singleton)
{
    CHECK(!LocationManager::getSingletonPtr());

    LocationManager* pLocationManager = new LocationManager();
    CHECK(pLocationManager == LocationManager::getSingletonPtr());

    delete pLocationManager;
    CHECK(!LocationManager::getSingletonPtr());
}


struct LocationEnvironment
{
    LocationEnvironment()
    {
        pLocationManager = new LocationManager();
    }

    ~LocationEnvironment()
    {
        delete pLocationManager;
    }

    LocationManager* pLocationManager;
};


SUITE(LocationManager_Groups)
{
    TEST_FIXTURE(LocationEnvironment, OneGroup)
    {
        pLocationManager->addLocation("group1", ATHENA_CORE_UNITTESTS_DATA_PATH);

        CHECK_EQUAL(1, pLocationManager->nbGroups());

        LocationManager::tGroupsList groups = pLocationManager->groups();

        CHECK_EQUAL(1, groups.size());
        CHECK_EQUAL("group1", groups[0]);
    }


    TEST_FIXTURE(LocationEnvironment, TwoGroups)
    {
        pLocationManager->addLocation("group1", ATHENA_CORE_UNITTESTS_DATA_PATH);
        pLocationManager->addLocation("group2", ATHENA_CORE_UNITTESTS_DATA_PATH);

        CHECK_EQUAL(2, pLocationManager->nbGroups());

        LocationManager::tGroupsList groups = pLocationManager->groups();

        CHECK_EQUAL(2, groups.size());
        CHECK((groups[0] == "group1") || (groups[0] == "group2"));
        CHECK((groups[1] == "group1") || (groups[1] == "group2"));
        CHECK(groups[0] != groups[1]);
    }
}


SUITE(LocationManager_Locations)
{
    TEST_FIXTURE(LocationEnvironment, UnknownGroup)
    {
        LocationManager::tLocationsList locations = pLocationManager->locations("unknown");

        CHECK_EQUAL(0, locations.size());
    }


    TEST_FIXTURE(LocationEnvironment, OneLocation)
    {
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH);

        LocationManager::tLocationsList locations = pLocationManager->locations("default");

        CHECK_EQUAL(1, locations.size());
        CHECK_EQUAL(ATHENA_CORE_UNITTESTS_DATA_PATH, locations[0]);
    }


    TEST_FIXTURE(LocationEnvironment, TwoLocations)
    {
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH);
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH + string(".."));

        LocationManager::tLocationsList locations = pLocationManager->locations("default");

        CHECK_EQUAL(2, locations.size());
        CHECK_EQUAL(ATHENA_CORE_UNITTESTS_DATA_PATH, locations[0]);
        CHECK_EQUAL(ATHENA_CORE_UNITTESTS_DATA_PATH + string(".."), locations[1]);
    }


    TEST_FIXTURE(LocationEnvironment, TwoGroups)
    {
        pLocationManager->addLocation("group1", ATHENA_CORE_UNITTESTS_DATA_PATH);
        pLocationManager->addLocation("group2", ATHENA_CORE_UNITTESTS_DATA_PATH + string(".."));

        LocationManager::tLocationsList locations = pLocationManager->locations("group1");

        CHECK_EQUAL(1, locations.size());
        CHECK_EQUAL(ATHENA_CORE_UNITTESTS_DATA_PATH, locations[0]);

        locations = pLocationManager->locations("group2");

        CHECK_EQUAL(1, locations.size());
        CHECK_EQUAL(ATHENA_CORE_UNITTESTS_DATA_PATH + string(".."), locations[0]);
    }
}


SUITE(LocationManager_Paths)
{
    TEST_FIXTURE(LocationEnvironment, UnknownGroup)
    {
        string path = pLocationManager->path("unknown", "lines.txt");

        CHECK_EQUAL("", path);
    }


    TEST_FIXTURE(LocationEnvironment, UnknownFile)
    {
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH);

        string path = pLocationManager->path("default", "unknown.txt");

        CHECK_EQUAL("", path);
    }


    TEST_FIXTURE(LocationEnvironment, ExistingFile)
    {
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH);

        string path = pLocationManager->path("default", "lines.txt");

        CHECK_EQUAL(string(ATHENA_CORE_UNITTESTS_DATA_PATH) + "lines.txt", path);
    }
}


SUITE(LocationManager_DataStream)
{
    TEST_FIXTURE(LocationEnvironment, UnknownGroup)
    {
        DataStream* pStream = pLocationManager->open("unknown", "lines.txt");

        CHECK(!pStream);
    }


    TEST_FIXTURE(LocationEnvironment, UnknownFile)
    {
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH);

        DataStream* pStream = pLocationManager->open("default", "unknown.txt");

        CHECK(!pStream);
    }


    TEST_FIXTURE(LocationEnvironment, ExistingFile)
    {
        pLocationManager->addLocation("default", ATHENA_CORE_UNITTESTS_DATA_PATH);

        DataStream* pStream = pLocationManager->open("default", "lines.txt");

        CHECK(pStream);

        delete pStream;
    }
}
