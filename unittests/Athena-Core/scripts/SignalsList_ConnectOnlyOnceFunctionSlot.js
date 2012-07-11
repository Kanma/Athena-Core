var nbCalls = 0;

function slot(value)
{
    nbCalls++;
}


signals = new Athena.Signals.SignalsList();

signals.connect(1, slot);
signals.connect(1, slot);

CHECK(!signals.empty, "!signals.empty");
CHECK_EQUAL(0, nbCalls);

signals.fire(1);

CHECK_EQUAL(1, nbCalls);

signals.disconnect(1, slot);

CHECK(signals.empty, "signals.empty");
