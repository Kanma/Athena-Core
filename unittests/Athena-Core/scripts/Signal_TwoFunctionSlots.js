var nbCalls = 0;

function slot1(value)
{
    nbCalls++;
}


function slot2(value)
{
    nbCalls++;
}


signal = new Athena.Signals.Signal();

signal.connect(slot1);
signal.connect(slot2);

CHECK_EQUAL(0, nbCalls);

signal.fire();

CHECK_EQUAL(2, nbCalls);
