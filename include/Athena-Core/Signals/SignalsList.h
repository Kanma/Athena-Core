/**	@file	SignalsList.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Signals::SignalsList'
*/

#ifndef _ATHENA_SIGNALS_SIGNALSLIST_H_
#define _ATHENA_SIGNALS_SIGNALSLIST_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Signals/Signal.h>
#include <Athena-Core/Utils/Iterators.h>

namespace Athena {
namespace Signals {


//----------------------------------------------------------------------------------------
/// @brief	Represents a list of signals, each identified by an ID
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL SignalsList
{
	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    //------------------------------------------------------------------------------------
	SignalsList();

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	~SignalsList();


	//_____ Function slots management __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Connect a function to a signal
    ///
    /// @param  id      ID of the signal
    /// @param  pSlot   The function representing the slot
    //------------------------------------------------------------------------------------
	void connect(tSignalID id, tSlot* pSlot);

    //------------------------------------------------------------------------------------
    /// @brief	Disconnect a function form a signal
    ///
    /// @param  id      ID of the signal
    /// @param  pSlot   The function representing the slot
    //------------------------------------------------------------------------------------
	void disconnect(tSignalID id, tSlot* pSlot);


	//_____ Method slots management __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Connect a method to a signal
    ///
    /// @param  id          ID of the signal
    /// @param  pObject     The object that is interested by the signal
    /// @param  pMethod     The method representing the slot
    //------------------------------------------------------------------------------------
	template<typename T>
	void connect(tSignalID id, T* pObject, void (T::*pMethod)(Utils::Variant*))
    {
		assert(pMethod);
		assert(pObject);

		tSignalsNativeIterator iter = m_signals.find(id);
		if (iter != m_signals.end())
		{
			iter->second->connect(pObject, pMethod);
		}
		else
		{
			Signal* pSignal = new Signal();
			pSignal->connect(pObject, pMethod);
			m_signals[id] = pSignal;
		}
	}

    //------------------------------------------------------------------------------------
    /// @brief	Disconnect a method from a signal
    ///
    /// @param  id          ID of the signal
    /// @param  pObject     The object that isn't interested by the signal anymore
    /// @param  pMethod     The method representing the slot
    //------------------------------------------------------------------------------------
	template<typename T>
    void disconnect(tSignalID id, T* pObject, void (T::*pMethod)(Utils::Variant*))
    {
		assert(pMethod);
		assert(pObject);

		tSignalsNativeIterator iter = m_signals.find(id);
		if (iter != m_signals.end())
		{
			iter->second->disconnect(pObject, pMethod);
			if (iter->second->isDisconnected())
			{
				delete iter->second;
				m_signals.erase(iter);
			}
		}
	}


// #if ATHENA_CORE_SCRIPTING
// 
//  //_____ Python slots management __________
// public:
//  void connect(tSignalID id, void* pPythonCallable);
//  void disconnect(tSignalID id, void* pPythonCallable);
// 
//  void connect(tSignalID id, void* pPythonObject, void* pMethod);
//  void disconnect(tSignalID id, void* pPythonObject, void* pMethod);
// 
// #endif


	//_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Fire a signal
    ///
    /// @param  id      ID of the signal
    /// @param  pValue  Parameter of the signal, will be handed to the connected slots
    //------------------------------------------------------------------------------------
	void fire(tSignalID id, Utils::Variant* pValue = 0) const;

    //------------------------------------------------------------------------------------
    /// @brief	Indicates if the list is empty (no signal)
    //------------------------------------------------------------------------------------
	inline bool isEmpty() const
	{
		return m_signals.empty();
	}


	//_____ Internal types __________
private:
    typedef std::map<tSignalID, Signal*>        tSignalsList;
    typedef Utils::MapIterator<tSignalsList>    tSignalsIterator;
    typedef tSignalsList::iterator              tSignalsNativeIterator;


	//_____ Attributes __________
private:
	tSignalsList m_signals;
};

}
}

#endif
