#include <UnitTest++.h>
#include <Athena-Core/Signals/Signal.h>

using namespace Athena::Signals;
using namespace Athena::Utils;


static int nbCalls = 0;


struct SignalEnvironment
{
    SignalEnvironment()
    {
        nbCalls = 0;
    }

    ~SignalEnvironment()
    {
    }
};


void functionSlot(Variant* pValue)
{
    ++nbCalls;
}

void functionSlot2(Variant* pValue)
{
    // Note: functionSlot2 must be sufficiently different of functionSlot to avoid an optimisation
    // in Release mode that makes the test 'Signal_TwoFunctionSlots' fail
    if (!pValue)
        ++nbCalls;
}


class CSlot
{
public:
    void methodSlot(Variant* pValue)
    {
        ++nbCalls;
    }
};


SUITE(SignalTests)
{
    TEST(ConnectDisconnectFunctionSlot)
    {
        Signal signal;

        CHECK(signal.isDisconnected());

        signal.connect((tSlot*) functionSlot);

        CHECK(!signal.isDisconnected());

        signal.disconnect((tSlot*) functionSlot);

        CHECK(signal.isDisconnected());
    }


    TEST_FIXTURE(SignalEnvironment, FunctionSlot)
    {
        Signal signal;

        signal.connect((tSlot*) functionSlot);

        CHECK_EQUAL(0, nbCalls);

        signal.fire();

        CHECK_EQUAL(1, nbCalls);
    }


    TEST_FIXTURE(SignalEnvironment, TwoFunctionSlots)
    {
        Signal signal;

        signal.connect((tSlot*) functionSlot);
        signal.connect((tSlot*) functionSlot2);

        CHECK_EQUAL(0, nbCalls);

        signal.fire();

        CHECK_EQUAL(2, nbCalls);
    }


    TEST_FIXTURE(SignalEnvironment, ConnectOnlyOnceFunctionSlot)
    {
        Signal signal;

        CHECK(signal.isDisconnected());

        signal.connect((tSlot*) functionSlot);
        signal.connect((tSlot*) functionSlot);

        CHECK(!signal.isDisconnected());
        CHECK_EQUAL(0, nbCalls);

        signal.fire();

        CHECK_EQUAL(1, nbCalls);

        signal.disconnect((tSlot*) functionSlot);

        CHECK(signal.isDisconnected());
    }


    TEST(ConnectDisconnectMethodSlot)
    {
        Signal signal;
        CSlot    slot;

        CHECK(signal.isDisconnected());

        signal.connect(&slot, &CSlot::methodSlot);

        CHECK(!signal.isDisconnected());

        signal.disconnect(&slot, &CSlot::methodSlot);

        CHECK(signal.isDisconnected());
    }


    TEST_FIXTURE(SignalEnvironment, MethodSlot)
    {
        Signal signal;
        CSlot    slot;

        signal.connect(&slot, &CSlot::methodSlot);

        CHECK_EQUAL(0, nbCalls);

        signal.fire();

        CHECK_EQUAL(1, nbCalls);
    }


    TEST_FIXTURE(SignalEnvironment, TwoMethodSlots)
    {
        Signal signal;
        CSlot    slot;
        CSlot    slot2;

        signal.connect(&slot, &CSlot::methodSlot);
        signal.connect(&slot2, &CSlot::methodSlot);

        CHECK_EQUAL(0, nbCalls);

        signal.fire();

        CHECK_EQUAL(2, nbCalls);
    }


    TEST_FIXTURE(SignalEnvironment, ConnectOnlyOneMethodSlot)
    {
        Signal signal;
        CSlot    slot;

        CHECK(signal.isDisconnected());

        signal.connect(&slot, &CSlot::methodSlot);
        signal.connect(&slot, &CSlot::methodSlot);

        CHECK(!signal.isDisconnected());
        CHECK_EQUAL(0, nbCalls);

        signal.fire();

        CHECK_EQUAL(1, nbCalls);

        signal.disconnect(&slot, &CSlot::methodSlot);

        CHECK(signal.isDisconnected());
    }
}
