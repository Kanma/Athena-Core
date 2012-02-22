function slot(value)
{
}


signal = new Athena.Signals.Signal();

CHECK(signal.isDisconnected(), "signal.isDisconnected()");

signal.connect(slot);

CHECK(!signal.isDisconnected(), "!signal.isDisconnected()");

signal.disconnect(slot);

CHECK(signal.isDisconnected(), "signal.isDisconnected()");
