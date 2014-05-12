Slot = function()
{
}

Slot.prototype.fired = function(value)
{
}


signal = new Athena.Signals.Signal();
slot = new Slot();

CHECK(signal.disconnected, "signal.disconnected");

signal.connect(slot, slot.fired);

CHECK(!signal.disconnected, "!signal.disconnected");

signal.disconnect(slot, slot.fired);

CHECK(signal.disconnected, "signal.disconnected");
