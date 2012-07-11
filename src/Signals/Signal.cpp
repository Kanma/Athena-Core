/** @file   Signal.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Signals::Signal'
*/

#include <Athena-Core/Signals/Signal.h>
#include <Athena-Core/Utils/Variant.h>

#if ATHENA_CORE_SCRIPTING
    #include <Athena-Core/Scripting.h>
#endif


using namespace Athena::Signals;
using namespace Athena::Utils;

#if ATHENA_CORE_SCRIPTING
    using namespace v8;
#endif


/****************************** CONSTRUCTION / DESTRUCTION *****************************/

Signal::Signal()
: m_bFiring(false)
{
}

//-----------------------------------------------------------------------

Signal::~Signal()
{
    tSlotsIterator iter(m_slots);
    while (iter.hasMoreElements())
    {
        tInternalSlot* pSlot = iter.peekNextPtr();

        if (pSlot->type == tInternalSlot::SLOT_METHOD)
            delete pSlot->pMethod;

        iter.moveNext();
    }
}


/*********************************** SLOTS MANAGEMENT **********************************/

void Signal::connect(tSlot* pSlot)
{
    assert(pSlot);

    // Check that the slot isn't already in the list
    tSlotsIterator iter(m_slots);
    while (iter.hasMoreElements())
    {
        tInternalSlot* pSlot2 = iter.peekNextPtr();
        if ((pSlot2->type == tInternalSlot::SLOT_FUNCTION) && (pSlot2->pFunction == pSlot))
            return;

        iter.moveNext();
    }

    tInternalSlot intSlot;
    intSlot.type = tInternalSlot::SLOT_FUNCTION;
    intSlot.pFunction = pSlot;

    if (!m_bFiring)
        m_slots.push_back(intSlot);
    else
        m_slotsToConnect.push_back(intSlot);
}

//-----------------------------------------------------------------------

void Signal::disconnect(tSlot* pSlot)
{
    assert(pSlot);

    tSlotsNativeIterator iter, iterEnd;
    for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
    {
        if ((iter->type == tInternalSlot::SLOT_FUNCTION) && (iter->pFunction == pSlot))
        {
            if (!m_bFiring)
            {
                m_slots.erase(iter);
            }
            else
            {
                tInternalSlot intSlot;
                intSlot.type = tInternalSlot::SLOT_FUNCTION;
                intSlot.pFunction = pSlot;

                m_slotsToDisconnect.push_back(intSlot);
            }
            return;
        }
    }
}

//-----------------------------------------------------------------------

void Signal::disconnect(IMethodCallback* pMethod)
{
    assert(pMethod);

    tSlotsNativeIterator iter, iterEnd;
    for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
    {
        if (iter->type == tInternalSlot::SLOT_METHOD)
        {
            if (iter->pMethod == pMethod)
            {
                if (!m_bFiring)
                {
                    delete iter->pMethod;
                    m_slots.erase(iter);
                }
                else
                {
                    tInternalSlot intSlot;
                    intSlot.type = tInternalSlot::SLOT_METHOD;
                    intSlot.pMethod = pMethod;

                    m_slotsToDisconnect.push_back(intSlot);
                }
                return;
            }
        }
    }
}


/****************************** JAVASCRIPT SLOTS MANAGEMENT ****************************/

#if ATHENA_CORE_SCRIPTING

void Signal::connect(v8::Persistent<v8::Object> function)
{
    assert(!function.IsEmpty());

    // Check that the slot isn't already in the list
    tSlotsNativeIterator iter, iterEnd;
    for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
    {
        if ((iter->type == tInternalSlot::SLOT_JS_FUNCTION) && (iter->js.function == function))
            return;
    }

    tInternalSlot intSlot;
    intSlot.type = tInternalSlot::SLOT_JS_FUNCTION;
    intSlot.js.function = function;

    if (!m_bFiring)
        m_slots.push_back(intSlot);
    else
        m_slotsToConnect.push_back(intSlot);
}

//-----------------------------------------------------------------------

void Signal::disconnect(v8::Persistent<v8::Object> function)
{
    assert(!function.IsEmpty());

    tSlotsNativeIterator iter, iterEnd;
    for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
    {
        if ((iter->type == tInternalSlot::SLOT_JS_FUNCTION) && (iter->js.function == function))
        {
            if (!m_bFiring)
            {
                m_slots.erase(iter);
            }
            else
            {
                tInternalSlot intSlot;
                intSlot.type = tInternalSlot::SLOT_JS_FUNCTION;
                intSlot.js.function = function;

                m_slotsToDisconnect.push_back(intSlot);
            }

            return;
        }
    }
}

//-----------------------------------------------------------------------

void Signal::connect(v8::Persistent<v8::Object> object, v8::Persistent<v8::Object> function)
{
    assert(!function.IsEmpty());
    assert(!object.IsEmpty());

    // Check that the slot isn't already in the list
    tSlotsNativeIterator iter, iterEnd;
    for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
    {
        if ((iter->type == tInternalSlot::SLOT_JS_METHOD) &&
            (iter->js.object == object) && (iter->js.function == function))
            return;
    }

    tInternalSlot intSlot;
    intSlot.type        = tInternalSlot::SLOT_JS_METHOD;
    intSlot.js.object   = object;
    intSlot.js.function = function;

    if (!m_bFiring)
        m_slots.push_back(intSlot);
    else
        m_slotsToConnect.push_back(intSlot);
}

//-----------------------------------------------------------------------

void Signal::disconnect(v8::Persistent<v8::Object> object, v8::Persistent<v8::Object> function)
{
    assert(!function.IsEmpty());
    assert(!object.IsEmpty());


    tSlotsNativeIterator iter, iterEnd;
    for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
    {
        if ((iter->type == tInternalSlot::SLOT_JS_METHOD) &&
            (iter->js.object == object) && (iter->js.function == function))
        {
            if (!m_bFiring)
            {
                m_slots.erase(iter);
            }
            else
            {
                tInternalSlot intSlot;
                intSlot.type        = tInternalSlot::SLOT_JS_METHOD;
                intSlot.js.object   = object;
                intSlot.js.function = function;

                m_slotsToDisconnect.push_back(intSlot);
            }

            return;
        }
    }
}

#endif


/*************************************** METHODS ***************************************/

void Signal::fire(Variant* pValue)
{
    // Fire the signal
    m_bFiring = true;

    tSlotsIterator iter(m_slots);
    while (iter.hasMoreElements())
    {
        tInternalSlot* pSlot = iter.peekNextPtr();

        if (pSlot->type == tInternalSlot::SLOT_FUNCTION)
        {
            pSlot->pFunction(pValue);
        }
        else if (pSlot->type == tInternalSlot::SLOT_METHOD)
        {
            pSlot->pMethod->fire(pValue);
        }

#if ATHENA_CORE_SCRIPTING
        else if (pSlot->type == tInternalSlot::SLOT_JS_FUNCTION)
        {
            if (pValue)
            {
                HandleScope handle_scope;
                Handle<Value> value = toJavaScript(pValue);
                Function::Cast(*pSlot->js.function)->Call(pSlot->js.function, 1, &value);
            }
            else
            {
                Function::Cast(*pSlot->js.function)->Call(pSlot->js.function, 0, 0);
            }
        }
        else if (pSlot->type == tInternalSlot::SLOT_JS_METHOD)
        {
            if (pValue)
            {
                HandleScope handle_scope;
                Handle<Value> value = toJavaScript(pValue);
                Function::Cast(*pSlot->js.function)->Call(pSlot->js.object, 1,  &value);
            }
            else
            {
                Function::Cast(*pSlot->js.function)->Call(pSlot->js.object, 0, 0);
            }
        }
#endif

        iter.moveNext();
    }

    m_bFiring = false;


    // Disconnect the slots that asked for it during the signal firing
    tSlotsIterator iter2(m_slotsToDisconnect);
    while (iter2.hasMoreElements())
    {
        tInternalSlot* pSlot = iter2.peekNextPtr();

        if (pSlot->type == tInternalSlot::SLOT_FUNCTION)
            disconnect(pSlot->pFunction);
        else if (pSlot->type == tInternalSlot::SLOT_METHOD)
            disconnect(pSlot->pMethod);

#if ATHENA_CORE_SCRIPTING
         else if (pSlot->type == tInternalSlot::SLOT_JS_FUNCTION)
             disconnect(pSlot->js.function);
         else if (pSlot->type == tInternalSlot::SLOT_JS_METHOD)
             disconnect(pSlot->js.object, pSlot->js.function);
#endif

        iter2.moveNext();
    }
    m_slotsToDisconnect.clear();


    // Connect the slots that asked for it during the signal firing
    tSlotsIterator iter3(m_slotsToConnect);
    while (iter3.hasMoreElements())
    {
        tInternalSlot* pSlot = iter3.peekNextPtr();
        m_slots.push_back(*pSlot);
        iter3.moveNext();
    }
    m_slotsToConnect.clear();
}
