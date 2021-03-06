/** @file   Path.h
    @author Philip Abbet

    Definition of the class 'Athena::Utils::Path'
*/

#ifndef _ATHENA_UTILS_PATH_H
#define _ATHENA_UTILS_PATH_H

#include <Athena-Core/Prerequisites.h>

namespace Athena {
namespace Utils {

//----------------------------------------------------------------------------------------
/// @brief  Contains utility methods to manipulate file paths
//----------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL Path
{
    //_____ Static methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Returns the current working directory
    //------------------------------------------------------------------------------------
    static std::string getCurrentWorkingDirectory();

    //------------------------------------------------------------------------------------
    /// @brief  Indicates if the provided path is absolute
    //------------------------------------------------------------------------------------
    static bool isAbsolute(const std::string& strPath);
};

}
}

#endif
