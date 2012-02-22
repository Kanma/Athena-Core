Slot = function()
{
}

Slot.prototype.fired = function(value)
{
}


signals = new Athena.Signals.SignalsList();
slot = new Slot();

CHECK(signals.isEmpty(), "signals.isEmpty()");

signals.connect(1, slot, slot.fired);

CHECK(!signals.isEmpty(), "!signals.isEmpty()");

signals.disconnect(1, slot, slot.fired);

CHECK(signals.isEmpty(), "signals.isEmpty()");
