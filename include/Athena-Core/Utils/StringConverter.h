/** @file   StringConverter.h
    @author Philip Abbet

    Declaration of the class 'Athena::Utils::StringConverter'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#ifndef _ATHENA_UTILS_STRINGCONVERTER_H
#define _ATHENA_UTILS_STRINGCONVERTER_H

#include <Athena-Core/Prerequisites.h>
#include <iostream>


namespace Athena {
namespace Utils {

//---------------------------------------------------------------------------------------
/// @brief  Contains conversions methods to/from string
//---------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL StringConverter
{
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Converts a Real to a string
    //-----------------------------------------------------------------------------------
    static std::string toString(Math::Real val, unsigned short precision = 6,
                                unsigned short width = 0, char fill = ' ',
                                std::ios::fmtflags flags = std::ios::fmtflags(0));

    //-----------------------------------------------------------------------------------
    /// @brief  Converts an int to a string
    //-----------------------------------------------------------------------------------
    static std::string toString(int val, unsigned short width = 0, char fill = ' ',
                                std::ios::fmtflags flags = std::ios::fmtflags(0));

    //-----------------------------------------------------------------------------------
    /// @brief  Converts an unsigned int to a string
    //-----------------------------------------------------------------------------------
    static std::string toString(unsigned int val, unsigned short width = 0, char fill = ' ',
                                std::ios::fmtflags flags = std::ios::fmtflags(0));

    //-----------------------------------------------------------------------------------
    /// @brief  Converts an unsigned long to a string
    //-----------------------------------------------------------------------------------
    static std::string toString(unsigned long val, unsigned short width = 0, char fill = ' ',
                                std::ios::fmtflags flags = std::ios::fmtflags(0));

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a long to a string
    //-----------------------------------------------------------------------------------
    static std::string toString(long val, unsigned short width = 0, char fill = ' ',
                                std::ios::fmtflags flags = std::ios::fmtflags(0));

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a boolean to a string
    ///
    /// @param  yesNo   If set, result is 'yes' or 'no' instead of 'true' or 'false'
    //-----------------------------------------------------------------------------------
    static std::string toString(bool val, bool yesNo = false);

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a string to a Real
    ///
    /// @return 0.0 if the value could not be parsed, otherwise the Real version of the
    ///         string
    //-----------------------------------------------------------------------------------
    static Math::Real parseReal(const std::string& val);

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a string to an int
    ///
    /// @return 0 if the value could not be parsed, otherwise the Real version of the
    ///         string
    //-----------------------------------------------------------------------------------
    static int parseInt(const std::string& val);

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a string to an unsigned int
    ///
    /// @return 0 if the value could not be parsed, otherwise the Real version of the
    ///         string
    //-----------------------------------------------------------------------------------
    static unsigned int parseUnsignedInt(const std::string& val);

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a string to a long
    ///
    /// @return 0 if the value could not be parsed, otherwise the Real version of the
    ///         string
    //-----------------------------------------------------------------------------------
    static long parseLong(const std::string& val);

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a string to an unsigned long
    ///
    /// @return 0 if the value could not be parsed, otherwise the Real version of the
    ///         string
    //-----------------------------------------------------------------------------------
    static unsigned long parseUnsignedLong(const std::string& val);

    //-----------------------------------------------------------------------------------
    /// @brief  Converts a string to a boolean
    ///
    /// @return true if case-insensitive match of the start of the string matches "true",
    ///         "yes" or "1", false otherwise
    //-----------------------------------------------------------------------------------
    static bool parseBool(const std::string& val);

    //-----------------------------------------------------------------------------------
    /// @brief  Indicates if the string contains a number
    //-----------------------------------------------------------------------------------
    static bool isNumber(const std::string& val);
};

}
}

#endif
