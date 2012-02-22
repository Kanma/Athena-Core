/** @file   StringUtils.h
    @author Philip Abbet

    Declaration of the class 'Athena::Utils::StringUtils'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#ifndef _ATHENA_UTILS_STRINGUTILS_H
#define _ATHENA_UTILS_STRINGUTILS_H

#include <Athena-Core/Prerequisites.h>


namespace Athena {
namespace Utils {

//----------------------------------------------------------------------------------------
/// @brief    Contains utility methods to manipulate strings
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL StringUtils
{
    //_____ Public types __________
public:
    typedef std::vector<std::string> tStringsList;


    //_____ Static methods __________
public:
    //------------------------------------------------------------------------------------
      /// @brief  Replace all instances of a sub-string with a another sub-string
       ///
      /// @param[in/out]  strSource   The string we work on
      /// @param          strWhat     Sub-string to find and replace
      /// @param          strWith     Sub-string to replace with (the new sub-string)
    //------------------------------------------------------------------------------------
    static void replaceAll(std::string &strSource, const std::string& strWhat,
                           const std::string& strWith);

    //------------------------------------------------------------------------------------
      /// @brief  Returns a list of strings containing all the substrings delimited by the
      ///         characters in the passed <code>delims</code> argument
       ///
      /// @param str      Source string
      /// @param delims   A list of delimiter characters to split by
      /// @return         The list of substrings
    //------------------------------------------------------------------------------------
    static tStringsList split(const std::string& str, const std::string& delims = "\t\n ");

    //------------------------------------------------------------------------------------
      /// @brief  Upper-cases all the characters in the string
    //------------------------------------------------------------------------------------
    static void toLowerCase(std::string& str);

    //------------------------------------------------------------------------------------
      /// @brief  Lower-cases all the characters in the string
    //------------------------------------------------------------------------------------
    static void toUpperCase(std::string& str);

    //------------------------------------------------------------------------------------
      /// @brief  Indicates whether the string begins with the pattern passed in
       ///
      /// @param str          Source string
      /// @param pattern      The pattern to compare with
      /// @param lowerCase    If true, the start of the string will be lower cased before
      ///                     comparison, pattern should also be in lower case.
    //------------------------------------------------------------------------------------
    static bool startsWith(const std::string& str, const std::string& pattern,
                           bool lowerCase = true);

    //------------------------------------------------------------------------------------
      /// @brief  Indicates whether the string ends with the pattern passed in
       ///
      /// @param str          Source string
      /// @param pattern      The pattern to compare with
      /// @param lowerCase    If true, the end of the string will be lower cased before
      ///                     comparison, pattern should also be in lower case.
    //------------------------------------------------------------------------------------
    static bool endsWith(const std::string& str, const std::string& pattern,
                         bool lowerCase = true);

    //------------------------------------------------------------------------------------
      /// @brief  Removes any whitespace characters, be it standard space or TABs and so on
       ///
    /// @remark The user may specify wether they want to trim only the beginning or the
    ///         end of the String (the default action is to trim both).
    //------------------------------------------------------------------------------------
    static void trim(std::string& str, bool left = true, bool right = true);
};

}
}

#endif
