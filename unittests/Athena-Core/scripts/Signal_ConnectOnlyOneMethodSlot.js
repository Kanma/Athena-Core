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


signal = new Athena.Signals.Signal();
slot = new Slot();

signal.connect(slot, slot.fired);
signal.connect(slot, slot.fired);

CHECK(!signal.isDisconnected(), "!signal.isDisconnected()");
CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, slot.nbCalls);

signal.fire();

CHECK_EQUAL(1, nbCalls);
CHECK_EQUAL(1, slot.nbCalls);

signal.disconnect(slot, slot.fired);

CHECK(signal.isDisconnected(), "signal.isDisconnected()");
