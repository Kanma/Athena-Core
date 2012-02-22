var nbCalls = 0;

Slot = function()
{
    this.nbCalls = 0;
}

Slot.prototype.fired = function(value)
{
    nbCalls++;
    this.nbCalls++;
}


signals = new Athena.Signals.SignalsList();
slot = new Slot();

signals.connect(1, slot, slot.fired);
signals.connect(1, slot, slot.fired);

CHECK(!signals.isEmpty(), "!signals.isEmpty()");
CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, slot.nbCalls);

signals.fire(1);

CHECK_EQUAL(1, nbCalls);
CHECK_EQUAL(1, slot.nbCalls);

signals.disconnect(1, slot, slot.fired);

CHECK(signals.isEmpty(), "signals.isEmpty()");
