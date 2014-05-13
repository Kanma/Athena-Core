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
slot1 = new Slot();
slot2 = new Slot();

signals.connect(1, slot1, slot1.fired);
signals.connect(2, slot2, slot2.fired);

CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, slot1.nbCalls);
CHECK_EQUAL(0, slot2.nbCalls);

signals.fire(1);

CHECK_EQUAL(1, nbCalls);
CHECK_EQUAL(1, slot1.nbCalls);
CHECK_EQUAL(0, slot2.nbCalls);

signals.fire(2);

CHECK_EQUAL(2, nbCalls);
CHECK_EQUAL(1, slot1.nbCalls);
CHECK_EQUAL(1, slot2.nbCalls);
