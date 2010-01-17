/**	@file	SignalsList.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Signals::SignalsList'
*/

#include <Athena-Core/Signals/SignalsList.h>
#include <Athena-Core/Utils/Variant.h>

using namespace Athena::Signals;
using namespace Athena::Utils;


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

void SignalsList::connect(tSignalID id, void* pPythonCallable)
{
	assert(pPythonCallable);

	tSignalsNativeIterator iter = m_signals.find(id);
	if (iter != m_signals.end())
	{
		iter->second->connect(pPythonCallable);
	}
	else
	{
		Signal* pSignal = new Signal();
		pSignal->connect(pPythonCallable);
		m_signals[id] = pSignal;
	}
}

//-----------------------------------------------------------------------

void SignalsList::disconnect(tSignalID id, void* pPythonCallable)
{
	assert(pPythonCallable);

	tSignalsNativeIterator iter = m_signals.find(id);
	if (iter != m_signals.end())
	{
		iter->second->disconnect(pPythonCallable);
		if (iter->second->isDisconnected())
		{
			delete iter->second;
			m_signals.erase(iter);
		}
	}
}

//-----------------------------------------------------------------------

void SignalsList::connect(tSignalID id, void* pPythonObject, void* pMethod)
{
	assert(pPythonObject);
	assert(pMethod);

	tSignalsNativeIterator iter = m_signals.find(id);
	if (iter != m_signals.end())
	{
		iter->second->connect(pPythonObject, pMethod);
	}
	else
	{
		Signal* pSignal = new Signal();
		pSignal->connect(pPythonObject, pMethod);
		m_signals[id] = pSignal;
	}
}

//-----------------------------------------------------------------------

void SignalsList::disconnect(tSignalID id, void* pPythonObject, void* pMethod)
{
	assert(pPythonObject);
	assert(pMethod);

	tSignalsNativeIterator iter = m_signals.find(id);
	if (iter != m_signals.end())
	{
		iter->second->disconnect(pPythonObject, pMethod);
		if (iter->second->isDisconnected())
		{
			delete iter->second;
			m_signals.erase(iter);
		}
	}
}

#endif


/*************************************** METHODS ***************************************/

void SignalsList::fire(tSignalID id, Utils::Variant* pValue) const
{
	tSignalsList::const_iterator iter = m_signals.find(id);
	if (iter != m_signals.end())
		iter->second->fire(pValue);
}
