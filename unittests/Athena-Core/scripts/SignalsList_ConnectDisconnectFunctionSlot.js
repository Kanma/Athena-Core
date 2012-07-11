function slot(value)
{
}


signals = new Athena.Signals.SignalsList();

CHECK(signals.empty, "signals.empty");

signals.connect(1, slot);

CHECK(!signals.empty, "!signals.empty");

signals.disconnect(1, slot);

CHECK(signals.empty, "signals.empty");
