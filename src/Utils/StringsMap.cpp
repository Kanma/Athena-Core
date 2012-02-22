/** @file   StringsMap.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Utils::StringsMap'
*/

#include <Athena-Core/Utils/StringsMap.h>
#include <assert.h>

using namespace Athena;
using namespace Athena::Utils;
using namespace std;


/****************************** CONSTRUCTION / DESTRUCTION *****************************/

StringsMap::StringsMap()
{
}

//-----------------------------------------------------------------------

StringsMap::~StringsMap()
{
    m_strings.clear();
}


/*************************************** METHODS ***************************************/

bool StringsMap::registerString(tID id, const std::string& strValue)
{
    assert(id != 0);

    if (!getString(id).empty() || (getID(strValue) != 0))
        return false;

    m_strings[id] = strValue;

    return true;
}

//-----------------------------------------------------------------------

tID StringsMap::registerString(const std::string& strValue)
{
    // Declarations
    tID id;

    if (getID(strValue) != 0)
        return 0;

    if (!m_strings.empty())
    {
        id = m_strings.rbegin()->first;
        while (!getString(id).empty())
            ++id;
    }
    else
    {
        id = 1;
    }

    m_strings[id] = strValue;

    return id;
}

//-----------------------------------------------------------------------

std::string StringsMap::getString(tID id)
{
    // Declarations
    tStringsMapIterator iter;

    iter = m_strings.find(id);
    if (iter != m_strings.end())
        return iter->second;

    return "";
}

//-----------------------------------------------------------------------

tID StringsMap::getID(const std::string& strValue)
{
    // Declarations
    tStringsMapIterator iter, iterEnd;

    for (iter = m_strings.begin(), iterEnd = m_strings.end(); iter != iterEnd; ++iter)
    {
        if (iter->second == strValue)
            return iter->first;
    }

    return 0;
}
