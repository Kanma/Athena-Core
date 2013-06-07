/** @file   LocationManager.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Data::LocationManager'
*/

#include <Athena-Core/Data/LocationManager.h>
#include <Athena-Core/Data/FileDataStream.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace Athena::Data;
using namespace Athena::Log;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS **************************************/

/// Context used for logging
static const char* __CONTEXT__ = "Location manager";

template<> LocationManager* Singleton<LocationManager>::ms_Singleton = 0;


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

LocationManager::LocationManager()
{
}

//-----------------------------------------------------------------------

LocationManager::~LocationManager()
{
}

//-----------------------------------------------------------------------

LocationManager& LocationManager::getSingleton()
{
    return Singleton<LocationManager>::getSingleton();
}

//-----------------------------------------------------------------------

LocationManager* LocationManager::getSingletonPtr()
{
    return Singleton<LocationManager>::getSingletonPtr();
}


/*************************************** METHODS ****************************************/

void LocationManager::addLocation(const std::string& strGroup, const std::string& strLocation)
{
    // Assertions
    assert(getSingletonPtr());
    assert(!strGroup.empty());
    assert(!strLocation.empty());

    // Ensure that the group exists
    if (m_groups.find(strGroup) == m_groups.end())
        m_groups[strGroup] = tLocationsList();

    // Add the location to the group
    m_groups[strGroup].push_back(strLocation);
}

//-----------------------------------------------------------------------

std::string LocationManager::path(const std::string& strGroup, const std::string& strFileName)
{
    // Assertions
    assert(getSingletonPtr());
    assert(!strGroup.empty());
    assert(!strFileName.empty());

    // Ensure that the group exists
    tGroupsNativeIterator group = m_groups.find(strGroup);
    if (group == m_groups.end())
        return "";

    // Search for the file in each location
    struct stat fileInfo;
    tLocationsIterator iter = tLocationsIterator(group->second.begin(), group->second.end());
    while (iter.hasMoreElements())
    {
        string strLocation = iter.getNext();

        if (stat((strLocation + strFileName).c_str(), &fileInfo) == 0)
            return strLocation + strFileName;
    }

    return "";
}

//-----------------------------------------------------------------------

DataStream* LocationManager::open(const std::string& strGroup, const std::string& strFileName)
{
    // Find the file
    string strPath = path(strGroup, strFileName);
    if (strPath.empty())
        return 0;

    // Open it
    FileDataStream* pStream = new FileDataStream(strPath, DataStream::READ);
    if (!pStream->isOpen())
    {
        delete pStream;
        pStream = 0;
    }

    return pStream;
}

//-----------------------------------------------------------------------

LocationManager::tLocationsList LocationManager::locations(const std::string& strGroup) const
{
    // Assertions
    assert(getSingletonPtr());
    assert(!strGroup.empty());

    // Ensure that the group exists
    tGroupsNativeConstIterator group = m_groups.find(strGroup);
    if (group == m_groups.end())
        return tLocationsList();

    return group->second;
}

//-----------------------------------------------------------------------

LocationManager::tGroupsList LocationManager::groups() const
{
    // Assertions
    assert(getSingletonPtr());

    tGroupsList list;

    // Ensure that the group exists
    tGroupsNativeConstIterator iter, iterEnd;
    for (iter = m_groups.begin(), iterEnd = m_groups.end(); iter != iterEnd; ++iter)
        list.push_back(iter->first);

    return list;
}
