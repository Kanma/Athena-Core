#include <UnitTest++.h>
#include <Athena-Core/Signals/SignalsUtils.h>
#include <Athena-Core/Signals/Declarations.h>

using namespace Athena::Signals;
using namespace Athena::Utils;
using namespace std;


const string SIGNAL_1 = "Signal1";
const string SIGNAL_2 = "Signal2";
const string SIGNAL_3 = "Signal3";


SUITE(SignalsUtilsTests)
{
    TEST(CreateSignalID)
    {
    	tSignalID id = SignalsUtils::getSignalID(SIGNAL_1);
    	CHECK(id >= SIGNAL_STRINGS);
    }


    TEST(RetrieveSignalID)
    {
    	tSignalID id1 = SignalsUtils::getSignalID(SIGNAL_2);
    	tSignalID id2 = SignalsUtils::getSignalID(SIGNAL_2);
    	CHECK_EQUAL(id1, id2);
    }


    TEST(RetrieveSignalName)
    {
    	tSignalID id = SignalsUtils::getSignalID(SIGNAL_3);
    	string strName = SignalsUtils::getSignalName(id);
    	CHECK_EQUAL(SIGNAL_3, strName);
    }


    TEST(RetrieveInvalidSignalName)
    {
    	string strName = SignalsUtils::getSignalName(SIGNAL_APPLICATION - 1);
    	CHECK_EQUAL("", strName);
    }
}
