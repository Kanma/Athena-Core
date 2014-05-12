var nbCalls = 0;

Slot = function()
{
    this.nbCalls = 0;
    this.value = 0;
}

Slot.prototype.fired = function(value)
{
    nbCalls++;
    this.nbCalls++;

    if (value !== undefined)
        this.value = value;
}


signals = new Athena.Signals.SignalsList();
slot = new Slot();

signals.connect(1, slot, slot.fired);

CHECK_EQUAL(0, nbCalls);
CHECK_EQUAL(0, slot.nbCalls);
CHECK_EQUAL(0, slot.value);

signals.fire(1);

CHECK_EQUAL(1, nbCalls);
CHECK_EQUAL(1, slot.nbCalls);
CHECK_EQUAL(0, slot.value);

signals.fire(1, 10);

CHECK_EQUAL(2, nbCalls);
CHECK_EQUAL(2, slot.nbCalls);
CHECK_EQUAL(10, slot.value);
