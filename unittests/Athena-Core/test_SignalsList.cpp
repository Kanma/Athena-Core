#include <UnitTest++.h>
#include <Athena-Core/Signals/SignalsList.h>

using namespace Athena::Signals;
using namespace Athena::Utils;


enum tSignals
{
	SIGNAL_1,
	SIGNAL_2
};


static int nbCallsSignal1 = 0;
static int nbCallsSignal2 = 0;


struct SignalsListEnvironment
{
	SignalsListEnvironment()
	{
		nbCallsSignal1 = 0;
		nbCallsSignal2 = 0;
	}

	~SignalsListEnvironment()
	{
	}
};


void SignalsList_functionSlot(Variant* pValue)
{
	++nbCallsSignal1;
}


class SignalsListSlot
{
public:
	tSignals signal;

	SignalsListSlot(tSignals theSignal)
	: signal(theSignal)
	{
	}

	void methodSlot(Variant* pValue)
	{
		if (signal == SIGNAL_1)
			++nbCallsSignal1;
		else
			++nbCallsSignal2;
	}
};


SUITE(SignalTests)
{
    TEST(SignalsList_ConnectDisconnectFunctionSlot)
    {
    	SignalsList list;

    	CHECK(list.isEmpty());

    	list.connect(SIGNAL_1, (tSlot*) SignalsList_functionSlot);

    	CHECK(!list.isEmpty());

    	list.disconnect(SIGNAL_1, (tSlot*) SignalsList_functionSlot);

    	CHECK(list.isEmpty());
    }


    TEST_FIXTURE(SignalsListEnvironment, SignalsList_FunctionSlot)
    {
    	SignalsList list;

    	list.connect(SIGNAL_1, (tSlot*) SignalsList_functionSlot);

    	CHECK_EQUAL(0, nbCallsSignal1);

    	list.fire(SIGNAL_1);

    	CHECK_EQUAL(1, nbCallsSignal1);
    }


    TEST_FIXTURE(SignalsListEnvironment, SignalsList_ConnectOnlyOnceFunctionSlot)
    {
    	SignalsList list;

    	CHECK(list.isEmpty());

    	list.connect(SIGNAL_1, (tSlot*) SignalsList_functionSlot);
    	list.connect(SIGNAL_1, (tSlot*) SignalsList_functionSlot);

    	CHECK(!list.isEmpty());
    	CHECK_EQUAL(0, nbCallsSignal1);

    	list.fire(SIGNAL_1);

    	CHECK_EQUAL(1, nbCallsSignal1);

    	list.disconnect(SIGNAL_1, (tSlot*) SignalsList_functionSlot);

    	CHECK(list.isEmpty());
    }


    TEST(SignalsList_ConnectDisconnectMethodSlot)
    {
    	SignalsList		list;
    	SignalsListSlot	slot(SIGNAL_1);

    	CHECK(list.isEmpty());

    	list.connect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);

    	CHECK(!list.isEmpty());

    	list.disconnect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);

    	CHECK(list.isEmpty());
    }


    TEST_FIXTURE(SignalsListEnvironment, SignalsList_MethodSlot)
    {
    	SignalsList		list;
    	SignalsListSlot	slot(SIGNAL_1);

    	list.connect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);

    	CHECK_EQUAL(0, nbCallsSignal1);

    	list.fire(SIGNAL_1);

    	CHECK_EQUAL(1, nbCallsSignal1);
    }


    TEST_FIXTURE(SignalsListEnvironment, SignalsList_TwoMethodSlots)
    {
    	SignalsList		list;
    	SignalsListSlot	slot(SIGNAL_1);
    	SignalsListSlot	slot2(SIGNAL_1);

    	list.connect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);
    	list.connect(SIGNAL_1, &slot2, &SignalsListSlot::methodSlot);

    	CHECK_EQUAL(0, nbCallsSignal1);

    	list.fire(SIGNAL_1);

    	CHECK_EQUAL(2, nbCallsSignal1);
    }


    TEST_FIXTURE(SignalsListEnvironment, SignalsList_TwoSignals)
    {
    	SignalsList		list;
    	SignalsListSlot	slot(SIGNAL_1);
    	SignalsListSlot	slot2(SIGNAL_2);

    	list.connect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);
    	list.connect(SIGNAL_2, &slot2, &SignalsListSlot::methodSlot);

    	CHECK_EQUAL(0, nbCallsSignal1);
    	CHECK_EQUAL(0, nbCallsSignal2);

    	list.fire(SIGNAL_1);

    	CHECK_EQUAL(1, nbCallsSignal1);
    	CHECK_EQUAL(0, nbCallsSignal2);

    	list.fire(SIGNAL_2);

    	CHECK_EQUAL(1, nbCallsSignal1);
    	CHECK_EQUAL(1, nbCallsSignal2);
    }


    TEST_FIXTURE(SignalsListEnvironment, SignalsList_ConnectOnlyOneMethodSlot)
    {
    	SignalsList		list;
    	SignalsListSlot	slot(SIGNAL_1);

    	CHECK(list.isEmpty());

    	list.connect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);
    	list.connect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);

    	CHECK(!list.isEmpty());
    	CHECK_EQUAL(0, nbCallsSignal1);

    	list.fire(SIGNAL_1);

    	CHECK_EQUAL(1, nbCallsSignal1);

    	list.disconnect(SIGNAL_1, &slot, &SignalsListSlot::methodSlot);

    	CHECK(list.isEmpty());
    }
}
