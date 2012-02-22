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
slot1 = new Slot();
slot2 = new Slot();

signal.connect(slot1, slot1.fired);
signal.connect(slot2, slot2.fired);

CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, slot1.nbCalls);
CHECK_EQUAL(0, slot2.nbCalls);

signal.fire();

CHECK_EQUAL(2, nbCalls);
CHECK_EQUAL(1, slot1.nbCalls);
CHECK_EQUAL(1, slot2.nbCalls);
