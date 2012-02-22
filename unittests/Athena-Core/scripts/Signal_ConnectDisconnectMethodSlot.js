Slot = function()
{
}

Slot.prototype.fired = function(value)
{
}


signal = new Athena.Signals.Signal();
slot = new Slot();

CHECK(signal.isDisconnected(), "signal.isDisconnected()");

signal.connect(slot, slot.fired);

CHECK(!signal.isDisconnected(), "!signal.isDisconnected()");

signal.disconnect(slot, slot.fired);

CHECK(signal.isDisconnected(), "signal.isDisconnected()");
