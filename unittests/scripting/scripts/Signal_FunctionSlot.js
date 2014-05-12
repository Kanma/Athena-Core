var nbCalls = 0;
var gValue = 0;

function slot(value)
{
    nbCalls++;

    if (value !== undefined)
        gValue = value;
}


signal = new Athena.Signals.Signal();

signal.connect(slot);

CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, gValue);

signal.fire();

CHECK_EQUAL(1, nbCalls);
CHECK_EQUAL(0, gValue);

signal.fire(10);

CHECK_EQUAL(2, nbCalls);
CHECK_EQUAL(10, gValue);
