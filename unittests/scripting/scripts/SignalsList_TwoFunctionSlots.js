var nbCalls = 0;

function slot1(value)
{
    nbCalls++;
}


function slot2(value)
{
    nbCalls++;
}


signals = new Athena.Signals.SignalsList();

signals.connect(1, slot1);
signals.connect(1, slot2);

CHECK_EQUAL(0, nbCalls);

signals.fire(1);

CHECK_EQUAL(2, nbCalls);
