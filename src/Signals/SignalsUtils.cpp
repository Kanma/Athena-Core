/**	@file	SignalsUtils.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Signals::SignalsUtils'
*/

#include <Athena-Core/Signals/SignalsUtils.h>
#include <Athena-Core/Signals/Declarations.h>

using namespace Athena::Signals;
using namespace Athena::Utils;


SignalsUtils::tIDsList SignalsUtils::m_ids;


/*************************************** METHODS ***************************************/

tSignalID SignalsUtils::getSignalID(const std::string& strName)
{
	// Assertions
	assert(!strName.empty());

	// Search the ID corresponding to the name
	tIDsNativeIterator iter = m_ids.find(strName);
	if (iter != m_ids.end())
		return iter->second;

	// Not found, generate one
	tSignalID id = SIGNAL_STRINGS + (tSignalID) m_ids.size();
	m_ids[strName] = id;
	return id;
}


std::string SignalsUtils::getSignalName(tSignalID id)
{
	// Assertions
	assert(id >= SIGNAL_STRINGS);
	assert(id < SIGNAL_APPLICATION);

	// Search the name corresponding to the ID
	tIDsIterator iter(m_ids.begin(), m_ids.end());
	while (iter.hasMoreElements())
	{
		if (iter.peekNextValue() == id)
			return iter.peekNextKey();

		iter.moveNext();
	}

	// Not found
	return "";
}
