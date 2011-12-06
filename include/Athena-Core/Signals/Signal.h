/**	@file	Signal.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Signals::Signal'
*/

#ifndef _ATHENA_SIGNALS_SIGNAL_H_
#define _ATHENA_SIGNALS_SIGNAL_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>

namespace Athena {
namespace Signals {


//----------------------------------------------------------------------------------------
/// @brief	Type of the function-slots
//----------------------------------------------------------------------------------------
typedef void tSlot(Utils::Variant*);


//----------------------------------------------------------------------------------------
/// @brief	Represents a signal
///
/// Signals and slots are used for communication between objects. It's an alternative to
/// the callback technique: a signal is fired when a particular event occurs, and a slot
/// is a function that is called in response to a particular signal. You can connect as
/// many signals as you want to a single slot, and a signal can be connected to as many
/// slots as you need.
///
/// Slots can be used for receiving signals, but they are also normal methods or functions.
///
/// Just as an object does not know if anything receives its signals, a slot does not know
/// if it has any signals connected to it. This ensures that truly independent components
/// can be created. Together, signals and slots make up a powerful component programming
/// mechanism.
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL Signal
{
	//_____ Internal types __________
private:
	class IMethodCallback
	{
	public:
		virtual ~IMethodCallback() {}
		virtual void fire(Utils::Variant* pValue) = 0;
	};

	template <class T> class MethodCallback: public IMethodCallback
	{
	public:
		T* m_pInstance;
		void (T::*m_callback)(Utils::Variant*);

	public:
		MethodCallback(T* pInstance, void (T::*callback)(Utils::Variant*))
		: m_pInstance(pInstance), m_callback(callback)
		{
		}

		virtual void fire(Utils::Variant* pValue)
		{
			(m_pInstance->*m_callback)(pValue);
		}
	};


	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    //------------------------------------------------------------------------------------
	Signal();

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	~Signal();


	//_____ Function slots management __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Connect a function to the signal
    ///
    /// @param  pSlot   The function representing the slot
    //------------------------------------------------------------------------------------
	void connect(tSlot* pSlot);

    //------------------------------------------------------------------------------------
    /// @brief	Disconnect a function form the signal
    ///
    /// @param  pSlot   The function representing the slot
    //------------------------------------------------------------------------------------
	void disconnect(tSlot* pSlot);


	//_____ Method slots management __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Connect a method to the signal
    ///
    /// @param  pObject     The object that is interested by the signal
    /// @param  pMethod     The method representing the slot
    //------------------------------------------------------------------------------------
	template<typename T>
    void connect(T* pObject, void (T::*pMethod)(Utils::Variant*))
    {
		assert(pMethod);
		assert(pObject);

		// Check that the slot isn't already in the list
		tSlotsNativeIterator iter, iterEnd;
		for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
		{
			if (iter->type == tInternalSlot::SLOT_METHOD)
			{
				MethodCallback<T>* pIntSlot = dynamic_cast<MethodCallback<T>*>(iter->pMethod);
				if (pIntSlot && (pIntSlot->m_pInstance == pObject) && (pIntSlot->m_callback == pMethod))
					return;
			}
		}

		tInternalSlot intSlot;
		intSlot.type = tInternalSlot::SLOT_METHOD;
		intSlot.pMethod = new MethodCallback<T>(pObject, pMethod);

		if (!m_bFiring)
			m_slots.push_back(intSlot);
		else
			m_slotsToConnect.push_back(intSlot);
	}

    //------------------------------------------------------------------------------------
    /// @brief	Disconnect a method from the signal
    ///
    /// @param  pObject     The object that isn't interested by the signal anymore
    /// @param  pMethod     The method representing the slot
    //------------------------------------------------------------------------------------
	template<typename T>
    void disconnect(T* pObject, void (T::*pMethod)(Utils::Variant*))
    {
		assert(pMethod);
		assert(pObject);

		tSlotsNativeIterator iter, iterEnd;
		for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
		{
			if (iter->type == tInternalSlot::SLOT_METHOD)
			{
				MethodCallback<T>* pIntSlot = dynamic_cast<MethodCallback<T>*>(iter->pMethod);
				if (pIntSlot && (pIntSlot->m_pInstance == pObject) && (pIntSlot->m_callback == pMethod))
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
						intSlot.pMethod = pIntSlot;

						m_slotsToDisconnect.push_back(intSlot);
					}
					return;
				}
			}
		}
	}

private:
    void disconnect(IMethodCallback* pMethod);


// #if ATHENA_CORE_SCRIPTING
// 
//  //_____ Python slots management __________
// public:
//     //------------------------------------------------------------------------------------
//     /// @brief   Connect a Python function to the signal
//     ///
//     /// @param  pSlot   The Python function representing the slot
//     //------------------------------------------------------------------------------------
//      void connect(void* pPythonFunction);
// 
//     //------------------------------------------------------------------------------------
//     /// @brief   Disconnect a Python function form the signal
//     ///
//     /// @param  pSlot   The Python function representing the slot
//     //------------------------------------------------------------------------------------
//  void disconnect(void* pPythonFunction);
// 
//     //------------------------------------------------------------------------------------
//     /// @brief   Connect a Python method to the signal
//     ///
//     /// @param  pObject   The Python object that is interested by the signal
//     /// @param  pMethod   The Python method representing the slot
//     //------------------------------------------------------------------------------------
//  void connect(void* pPythonObject, void* pMethod);
// 
//     //------------------------------------------------------------------------------------
//     /// @brief   Disconnect a Python method from the signal
//     ///
//     /// @param  pObject   The Python object that isn't interested by the signal anymore
//     /// @param  pMethod   The Python method representing the slot
//     //------------------------------------------------------------------------------------
//  void disconnect(void* pPythonObject, void* pMethod);
// 
// #endif


	//_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Fire the signal
    ///
    /// @param  pValue  Parameter of the signal, will be handed to the connected slots
    //------------------------------------------------------------------------------------
	void fire(Utils::Variant* pValue = 0);

    //------------------------------------------------------------------------------------
    /// @brief	Indicates if the signal isn't connected to any slot
    //------------------------------------------------------------------------------------
	inline bool isDisconnected() const
	{
		return m_slots.empty();
	}

    //------------------------------------------------------------------------------------
    /// @brief	Returns the parameter of the currently fired signal (only usefull for
    ///         Python slots)
    //------------------------------------------------------------------------------------
	static Utils::Variant* _getCurrentValue();


	//_____ Internal types __________
private:
	struct tInternalSlot
	{
		enum {
		    SLOT_FUNCTION,
		    SLOT_METHOD,
// #if ATHENA_CORE_SCRIPTING
//          SLOT_PYTHON_FUNCTION,
//          SLOT_PYTHON_METHOD
// #endif
		} type;

		union
		{
			tSlot*				pFunction;
			IMethodCallback*	pMethod;
// #if ATHENA_CORE_SCRIPTING
//          struct
//          {
//              void*           pCallable;
//              void*           pObject;
//          } python;
// #endif
		};
	};

    typedef std::vector<tInternalSlot>          tSlotsList;
    typedef Utils::VectorIterator<tSlotsList>   tSlotsIterator;
    typedef tSlotsList::iterator                tSlotsNativeIterator;


	//_____ Attributes __________
private:
	tSlotsList	            m_slots;                ///< The slots connected to the signal
	bool					m_bFiring;              ///< Indicates if the signal is currently fired
	tSlotsList	            m_slotsToDisconnect;    ///< Slots to disconnect when all the slots triggered
	tSlotsList	            m_slotsToConnect;       ///< Slots to connect when all the slots triggered

	static Utils::Variant*	m_pCurrentValue;        ///< Parameter of the signal currently fired
};

}
}

#endif
