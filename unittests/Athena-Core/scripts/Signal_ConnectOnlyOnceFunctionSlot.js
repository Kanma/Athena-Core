var nbCalls = 0;

function slot(value)
{
    nbCalls++;
}


signal = new Athena.Signals.Signal();

signal.connect(slot);
signal.connect(slot);

CHECK(!signal.isDisconnected(), "!signal.isDisconnected()");
CHECK_EQUAL(0, nbCalls);

signal.fire();

CHECK_EQUAL(1, nbCalls);

signal.disconnect(slot);

CHECK(signal.isDisconnected(), "signal.isDisconnected()");
