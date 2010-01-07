/** @file   Prerequisites.h
    @author Philip Abbet
    
    Declaration of the types of the Athena-Core module
*/

#ifndef _ATHENA_CORE_PREREQUISITES_H_
#define _ATHENA_CORE_PREREQUISITES_H_

#include <Athena-Core/Declarations.h>
#include <string>
#include <vector>
#include <map>

//---------------------------------------------------------------------------------------
/// @brief	Main namespace. All the components of the Athena engine belongs to this
///			namespace
//---------------------------------------------------------------------------------------
namespace Athena
{
	//-----------------------------------------------------------------------------------
	/// @brief	Contains all the utility classes
	//-----------------------------------------------------------------------------------
    namespace Utils
    {
        class Path;
        class StringsMap;
        class StringUtils;
		
		/// @brief Used to represent an ID
		typedef unsigned int tID;
    }
}

#endif
