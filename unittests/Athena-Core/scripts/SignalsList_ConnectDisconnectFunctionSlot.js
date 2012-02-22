function slot(value)
{
}


signals = new Athena.Signals.SignalsList();

CHECK(signals.isEmpty(), "signals.isEmpty()");

signals.connect(1, slot);

CHECK(!signals.isEmpty(), "!signals.isEmpty()");

signals.disconnect(1, slot);

CHECK(signals.isEmpty(), "signals.isEmpty()");
