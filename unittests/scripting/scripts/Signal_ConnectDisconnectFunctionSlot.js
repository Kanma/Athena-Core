function slot(value)
{
}


signal = new Athena.Signals.Signal();

CHECK(signal.disconnected, "signal.disconnected");

signal.connect(slot);

CHECK(!signal.disconnected, "!signal.disconnected");

signal.disconnect(slot);

CHECK(signal.disconnected, "signal.disconnected");
