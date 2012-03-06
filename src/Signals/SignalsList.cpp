/** @file   SignalsList.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Signals::SignalsList'
*/

#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Utils/Variant.h>

using namespace Athena::Signals;
using namespace Athena::Utils;

#if ATHENA_CORE_SCRIPTING
    using namespace v8;
#endif



/****************************** CONSTRUCTION / DESTRUCTION *****************************/

SignalsList::SignalsList()
{
}

//-----------------------------------------------------------------------

SignalsList::~SignalsList()
{
    // Declarations
    tSignalsNativeIterator iter, iterEnd;

    // Destroy the signals
    for (iter = m_signals.begin(), iterEnd = m_signals.end(); iter != iterEnd; ++iter)
        delete iter->second;
}


/*********************************** SLOTS MANAGEMENT **********************************/

void SignalsList::connect(tSignalID id, tSlot* pSlot)
{
    assert(pSlot);

    tSignalsNativeIterator iter = m_signals.find(id);
    if (iter != m_signals.end())
    {
        iter->second->connect(pSlot);
    }
    else
    {
        Signal* pSignal = new Signal();
        pSignal->connect(pSlot);
        m_signals[id] = pSignal;
    }
}

//-----------------------------------------------------------------------

void SignalsList::disconnect(tSignalID id, tSlot* pSlot)
{
    assert(pSlot);

    tSignalsNativeIterator iter = m_signals.find(id);
    if (iter != m_signals.end())
    {
        iter->second->disconnect(pSlot);
        if (iter->second->isDisconnected())
        {
            delete iter->second;
            m_signals.erase(iter);
        }
    }
}


/******************************** PYTHON SLOTS MANAGEMENT ******************************/

#if ATHENA_CORE_SCRIPTING

void SignalsList::connect(tSignalID id, Persistent<Object> function)
{
    assert(!function.IsEmpty());

    tSignalsNativeIterator iter = m_signals.find(id);
    if (iter != m_signals.end())
    {
        iter->second->connect(function);
    }
    else
    {
        Signal* pSignal = new Signal();
        pSignal->connect(function);
        m_signals[id] = pSignal;
    }
}

//-----------------------------------------------------------------------

void SignalsList::disconnect(tSignalID id, Persistent<Object> function)
{
    assert(!function.IsEmpty());

    tSignalsNativeIterator iter = m_signals.find(id);
    if (iter != m_signals.end())
    {
        iter->second->disconnect(function);
        if (iter->second->isDisconnected())
        {
            delete iter->second;
            m_signals.erase(iter);
        }
    }
}

//-----------------------------------------------------------------------

void SignalsList::connect(tSignalID id, Persistent<Object> object,
                          Persistent<Object> function)
{
    assert(!function.IsEmpty());
    assert(!object.IsEmpty());

    tSignalsNativeIterator iter = m_signals.find(id);
    if (iter != m_signals.end())
    {
        iter->second->connect(object, function);
    }
    else
    {
        Signal* pSignal = new Signal();
        pSignal->connect(object, function);
        m_signals[id] = pSignal;
    }
}

//-----------------------------------------------------------------------

void SignalsList::disconnect(tSignalID id, Persistent<Object> object,
                          Persistent<Object> function)
{
    assert(!function.IsEmpty());
    assert(!object.IsEmpty());

    tSignalsNativeIterator iter = m_signals.find(id);
    if (iter != m_signals.end())
    {
        iter->second->disconnect(object, function);
        if (iter->second->isDisconnected())
        {
            delete iter->second;
            m_signals.erase(iter);
        }
    }
}

#endif


/*************************************** METHODS ***************************************/

void SignalsList::fire(tSignalID id, Variant* pValue) const
{
    tSignalsList::const_iterator iter = m_signals.find(id);
    if (iter != m_signals.end())
        iter->second->fire(pValue);
}
