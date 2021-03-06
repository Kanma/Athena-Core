/** @file   LocationManager.h
    @author Philip Abbet

    Declaration of the class 'Athena::Data::LocationManager'
*/

#ifndef _ATHENA_DATA_LOCATIONMANAGER_H_
#define _ATHENA_DATA_LOCATIONMANAGER_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>


namespace Athena {
namespace Data {

//----------------------------------------------------------------------------------------
/// @brief  This class is used to store some locations
///
/// @remark This class is a singleton.
//----------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL LocationManager: public Utils::Singleton<LocationManager>
{
    //_____ Internal types __________
public:
    typedef std::vector<std::string>                tLocationsList;
    typedef Utils::VectorIterator<tLocationsList>   tLocationsIterator;
    typedef tLocationsList::iterator                tLocationsNativeIterator;

    typedef std::vector<std::string>                tGroupsList;

private:
    typedef std::map<std::string, tLocationsList>   tGroupsMap;
    typedef Utils::MapIterator<tGroupsMap>          tGroupsIterator;
    typedef tGroupsMap::iterator                    tGroupsNativeIterator;
    typedef tGroupsMap::const_iterator              tGroupsNativeConstIterator;


    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    //------------------------------------------------------------------------------------
    LocationManager();

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    ~LocationManager();

    //------------------------------------------------------------------------------------
    /// @brief  Returns a reference to the instance of the location manager
    /// @return A reference to the manager
    //------------------------------------------------------------------------------------
    static LocationManager& getSingleton();

    //------------------------------------------------------------------------------------
    /// @brief  Returns a pointer to the instance of the location manager
    /// @return A pointer to the manager
    //------------------------------------------------------------------------------------
    static LocationManager* getSingletonPtr();


    //_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Add the groups and locations defined by a JSON object
    ///
    /// @param  json_locations  The JSON declaration. The format is:
    ///                         {
    ///                             "group1": [ "location1", "location2" ],
    ///                             "group2": [ "location3", "location4" ]
    ///                         }
    //------------------------------------------------------------------------------------
    void addLocations(const rapidjson::Value& json_locations);

    //------------------------------------------------------------------------------------
    /// @brief  Add a location
    ///
    /// @param  strGroup        The group to which the location is added
    /// @param  strLocation     The location
    //------------------------------------------------------------------------------------
    void addLocation(const std::string& strGroup, const std::string& strLocation);

    //------------------------------------------------------------------------------------
    /// @brief  Retrieves the path to a file
    ///
    /// @param  strGroup        The group to look in
    /// @param  strFileName     The name of the file
    //------------------------------------------------------------------------------------
    std::string path(const std::string& strGroup, const std::string& strFileName);

    //------------------------------------------------------------------------------------
    /// @brief  Open a file (in reading mode)
    ///
    /// @param  strGroup        The group to look in
    /// @param  strFileName     The name of the file
    //------------------------------------------------------------------------------------
    DataStream* open(const std::string& strGroup, const std::string& strFileName);

    //------------------------------------------------------------------------------------
    /// @brief  Returns the locations associated to the given group
    ///
    /// @param  strGroup        The group
    //------------------------------------------------------------------------------------
    tLocationsList locations(const std::string& strGroup) const;

    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of groups
    //------------------------------------------------------------------------------------
    inline size_t nbGroups() const
    {
        return m_groups.size();
    }

    //------------------------------------------------------------------------------------
    /// @brief  Returns a list of the group names
    //------------------------------------------------------------------------------------
    tGroupsList groups() const;


    //_____ Attributes __________
private:
    tGroupsMap m_groups;        ///< The groups
};

}
}

#endif
