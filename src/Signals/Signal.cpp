/**	@file	Signal.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Signals::Signal'
*/

#include <Athena-Core/Signals/Signal.h>
#include <Athena-Core/Utils/Variant.h>


#if ATHENA_CORE_SCRIPTING
#   ifdef __APPLE__
#	    include <Python/Python.h>
#   else
#	    include <Python.h>
#   endif
#endif


using namespace Athena::Signals;
using namespace Athena::Utils;


/*********************************** STATIC ATTRIBUTES *********************************/

Variant* Signal::m_pCurrentValue = 0;


/****************************** CONSTRUCTION / DESTRUCTION *****************************/

Signal::Signal()
: m_bFiring(false)
{
}


//---------------------------------------------------------------------------------------
/// @brief	Destructor
//---------------------------------------------------------------------------------------
Signal::~Signal()
{
	tSlotsIterator iter(m_slots);
	while (iter.hasMoreElements())
	{
        tInternalSlot* pSlot = iter.peekNextPtr();
	    
		if (pSlot->type == tInternalSlot::SLOT_METHOD)
		{
			delete pSlot->pMethod;
		}

#if ATHENA_CORE_SCRIPTING

		else if (pSlot->type == tInternalSlot::SLOT_PYTHON_FUNCTION)
		{
			Py_XDECREF((PyObject*) pSlot->python.pCallable);
		}
		else if (pSlot->type == tInternalSlot::SLOT_PYTHON_METHOD)
		{
			Py_XDECREF((PyObject*) pSlot->python.pObject);
			Py_XDECREF((PyObject*) pSlot->python.pCallable);
		}

#endif

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


/******************************** PYTHON SLOTS MANAGEMENT ******************************/

#if ATHENA_CORE_SCRIPTING

void Signal::connect(void* pPythonFunction)
{
	assert(pPythonFunction);

	// Check that the slot isn't already in the list
	tSlotsNativeIterator iter, iterEnd;
	for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
	{
		if ((iter->type == tInternalSlot::SLOT_PYTHON_FUNCTION) && (iter->python.pCallable == pPythonFunction))
			return;
	}

	tInternalSlot intSlot;
	intSlot.type = tInternalSlot::SLOT_PYTHON_FUNCTION;
	intSlot.python.pCallable = pPythonFunction;

	if (!m_bFiring)
		m_slots.push_back(intSlot);
	else
		m_slotsToConnect.push_back(intSlot);

	Py_XINCREF((PyObject*) pPythonFunction);
}

//-----------------------------------------------------------------------

void Signal::disconnect(void* pPythonFunction)
{
	assert(pPythonFunction);

	tSlotsNativeIterator iter, iterEnd;
	for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
	{
		if ((iter->type == tInternalSlot::SLOT_PYTHON_FUNCTION) && (iter->python.pCallable == pPythonFunction))
		{
			if (!m_bFiring)
			{
				Py_XDECREF((PyObject*) iter->python.pCallable);
				m_slots.erase(iter);
			}
			else
			{
				tInternalSlot intSlot;
				intSlot.type = tInternalSlot::SLOT_PYTHON_FUNCTION;
				intSlot.python.pCallable = pPythonFunction;

				m_slotsToDisconnect.push_back(intSlot);
			}
			return;
		}
	}
}

//-----------------------------------------------------------------------

void Signal::connect(void* pPythonObject, void* pMethod)
{
	assert(pPythonObject);
	assert(pMethod);

	pMethod = PyMethod_Function((PyObject*) pMethod);

	// Check that the slot isn't already in the list
	tSlotsNativeIterator iter, iterEnd;
	for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
	{
		if ((iter->type == tInternalSlot::SLOT_PYTHON_METHOD) && (iter->python.pObject == pPythonObject) &&
			(iter->python.pCallable == pMethod))
			return;
	}

	tInternalSlot intSlot;
	intSlot.type = tInternalSlot::SLOT_PYTHON_METHOD;
	intSlot.python.pObject = pPythonObject;
	intSlot.python.pCallable = pMethod;

	if (!m_bFiring)
		m_slots.push_back(intSlot);
	else
		m_slotsToConnect.push_back(intSlot);

	Py_XINCREF((PyObject*) pPythonObject);
	Py_XINCREF((PyObject*) pMethod);
}

//-----------------------------------------------------------------------

void Signal::disconnect(void* pPythonObject, void* pMethod)
{
	assert(pPythonObject);
	assert(pMethod);

	pMethod = PyMethod_Function((PyObject*) pMethod);

	tSlotsNativeIterator iter, iterEnd;
	for (iter = m_slots.begin(), iterEnd = m_slots.end(); iter != iterEnd; ++iter)
	{
		if ((iter->type == tInternalSlot::SLOT_PYTHON_METHOD) && (iter->python.pObject == pPythonObject) &&
			(iter->python.pCallable == pMethod))
		{
			if (!m_bFiring)
			{
				Py_XDECREF((PyObject*) iter->python.pObject);
				Py_XDECREF((PyObject*) iter->python.pCallable);
				m_slots.erase(iter);
			}
			else
			{
				tInternalSlot intSlot;
				intSlot.type = tInternalSlot::SLOT_PYTHON_METHOD;
				intSlot.python.pObject = pPythonObject;
				intSlot.python.pCallable = pMethod;

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
	Variant* pPreviousValue = m_pCurrentValue;
	m_pCurrentValue = pValue;

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

		else if (pSlot->type == tInternalSlot::SLOT_PYTHON_FUNCTION)
		{
			PyObject* arglist = PyTuple_New(0);
			
			PyObject* result = PyEval_CallObject((PyObject*) pSlot->python.pCallable, arglist);
			Py_DECREF(arglist);

			if (result != 0)
				Py_DECREF(result);
		}
		else if (pSlot->type == tInternalSlot::SLOT_PYTHON_METHOD)
		{
			PyObject* arglist = PyTuple_Pack(1, (PyObject*) pSlot->python.pObject);
			
			PyObject* result = PyEval_CallObject((PyObject*) pSlot->python.pCallable, arglist);
			Py_DECREF(arglist);

			if (result != 0)
				Py_DECREF(result);
		}

#endif

        iter.moveNext();
	}

	m_bFiring = false;

	tSlotsIterator iter2(m_slotsToDisconnect);
	while (iter2.hasMoreElements())
	{
        tInternalSlot* pSlot = iter2.peekNextPtr();

		if (pSlot->type == tInternalSlot::SLOT_FUNCTION)
			disconnect(pSlot->pFunction);
		else if (pSlot->type == tInternalSlot::SLOT_METHOD)
			disconnect(pSlot->pMethod);

#if ATHENA_CORE_SCRIPTING

		else if (pSlot->type == tInternalSlot::SLOT_PYTHON_FUNCTION)
			disconnect(pSlot->python.pCallable);
		else if (pSlot->type == tInternalSlot::SLOT_PYTHON_METHOD)
			disconnect(pSlot->python.pObject, pSlot->python.pCallable);

#endif

        iter2.moveNext();
	}
	m_slotsToDisconnect.clear();

	tSlotsIterator iter3(m_slotsToConnect);
	while (iter3.hasMoreElements())
	{
        tInternalSlot* pSlot = iter3.peekNextPtr();
		m_slots.push_back(*pSlot);
        iter3.moveNext();
	}
	m_slotsToConnect.clear();

	delete m_pCurrentValue;
	m_pCurrentValue = pPreviousValue;
}

//-----------------------------------------------------------------------

Variant* Signal::_getCurrentValue()
{
	return m_pCurrentValue;
}
