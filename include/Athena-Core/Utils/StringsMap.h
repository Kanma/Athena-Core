/** @file   StringsMap.h
    @author Philip Abbet

    Declaration of the class 'Athena::Utils::StringsMap'
*/

#ifndef _ATHENA_UTILS_STRINGSMAP_H_
#define _ATHENA_UTILS_STRINGSMAP_H_

#include <Athena-Core/Prerequisites.h>


namespace Athena {
namespace Utils {

//---------------------------------------------------------------------------------------
/// @brief  Represents a map of strings
///
/// This class is used similarly than a hash-table. Each string is associated with an ID.
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL StringsMap
{
    //_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Constructor
    //-----------------------------------------------------------------------------------
    StringsMap();

    //-----------------------------------------------------------------------------------
    /// @brief  Destructor
    //-----------------------------------------------------------------------------------
    ~StringsMap();


    //_____ Methods __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Register a string by specifying its ID
    ///
    /// @param  id          The ID
    /// @param  strValue    The string
    /// @return             'true' if successful (ie. the ID was free)
    //-----------------------------------------------------------------------------------
    bool registerString(tID id, const std::string& strValue);

    //-----------------------------------------------------------------------------------
    /// @brief  Register a string, and let the map determines its ID
    ///
    /// @param  strValue    The string
    /// @return             The ID
    //-----------------------------------------------------------------------------------
    tID registerString(const std::string& strValue);

    //-----------------------------------------------------------------------------------
    /// @brief  Returns a string
    ///
    /// @param  id  ID of the string
    /// @return     The string, empty if the ID is invalid
    //-----------------------------------------------------------------------------------
    std::string getString(tID id);

    //-----------------------------------------------------------------------------------
    /// @brief  Returns the ID of a string
    ///
    /// @param  strValue    The string
    /// @return             The ID of the string
    //-----------------------------------------------------------------------------------
    tID getID(const std::string& strValue);


    //_____ Internal types ___________
private:
    typedef std::map<tID, std::string>  tStringsMap;
    typedef tStringsMap::iterator       tStringsMapIterator;


    //_____ Attributes ___________
private:
    tStringsMap m_strings;  ///< The map of strings
};

}
}

#endif
