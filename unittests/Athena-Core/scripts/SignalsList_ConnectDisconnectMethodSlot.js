Slot = function()
{
}

Slot.prototype.fired = function(value)
{
}


signals = new Athena.Signals.SignalsList();
slot = new Slot();

CHECK(signals.empty, "signals.empty");

signals.connect(1, slot, slot.fired);

CHECK(!signals.empty, "!signals.empty");

signals.disconnect(1, slot, slot.fired);

CHECK(signals.empty, "signals.empty");
