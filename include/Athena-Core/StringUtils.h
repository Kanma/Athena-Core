/**	@file	StringUtils.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Utils::StringUtils'
*/

#ifndef _ATHENA_UTILS_STRINGUTILS_H
#define _ATHENA_UTILS_STRINGUTILS_H

#include <Athena-Core/Prerequisites.h>


namespace Athena {
namespace Utils {

//---------------------------------------------------------------------------------------
/// @brief	Contains utility methods to manipulate strings
//---------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL StringUtils
{
    //_____ Public types __________
public:
    typedef std::vector<std::string> tStringsList;
    

	//_____ Static methods __________
public:
    //-----------------------------------------------------------------------------------
  	/// @brief  Replace all instances of a sub-string with a another sub-string
   	///  	
  	/// @param[in/out]  strSource   The string we work on
  	/// @param          strWhat     Sub-string to find and replace
  	/// @param          strWith     Sub-string to replace with (the new sub-string)
    //-----------------------------------------------------------------------------------
    static void replaceAll(std::string &strSource, const std::string& strWhat,
	                       const std::string& strWith);
	                       
    //-----------------------------------------------------------------------------------
  	/// @brief  Returns a list of strings containing all the substrings delimited by the
  	///         characters in the passed <code>delims</code> argument
   	///  	
  	/// @param str      Source string
  	/// @param delims   A list of delimiter characters to split by
  	/// @return         The list of substrings
    //-----------------------------------------------------------------------------------
	static tStringsList split(const std::string& str, const std::string& delims = "\t\n ");
};

}
}

#endif
