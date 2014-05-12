var nbCalls = 0;
var gValue = 0;

function slot(value)
{
    nbCalls++;

    if (value !== undefined)
        gValue = value;
}


signals = new Athena.Signals.SignalsList();

signals.connect(1, slot);

CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, gValue);

signals.fire(1);

CHECK_EQUAL(1, nbCalls);
CHECK_EQUAL(0, gValue);

signals.fire(1, 10);

CHECK_EQUAL(2, nbCalls);
CHECK_EQUAL(10, gValue);
