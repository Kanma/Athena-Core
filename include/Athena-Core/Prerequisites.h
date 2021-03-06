/** @file   Prerequisites.h
    @author Philip Abbet

    Declaration of the types of the Athena-Core module
*/

#ifndef _ATHENA_CORE_PREREQUISITES_H_
#define _ATHENA_CORE_PREREQUISITES_H_

#include <Athena-Math/Prerequisites.h>
#include <Athena-Core/Config.h>
#include <Athena-Core/Utils/Singleton.h>
#include <string>
#include <vector>
#include <map>
#include <list>


/// Used to export symbols from the library
#if (ATHENA_PLATFORM == ATHENA_PLATFORM_WIN32)
#    ifdef ATHENA_CORE_EXPORTS
#        define ATHENA_CORE_SYMBOL  __declspec(dllexport)
#    else
#        define ATHENA_CORE_SYMBOL  __declspec(dllimport)
#    endif
#else
#    define ATHENA_CORE_SYMBOL
#endif


//---------------------------------------------------------------------------------------
/// @brief  Main namespace. All the components of the Athena engine belongs to this
///         namespace
//---------------------------------------------------------------------------------------
namespace Athena
{
    //-----------------------------------------------------------------------------------
    /// @brief  Contains all the data-related classes
    //-----------------------------------------------------------------------------------
    namespace Data
    {
        class DataStream;
        class FileDataStream;
        class LocationManager;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Contains all the logging-related classes
    //-----------------------------------------------------------------------------------
    namespace Log
    {
        class LogManager;
        class ILogListener;
        class ConsoleLogListener;
        class XMLLogListener;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Contains all the signals-related classes
    //-----------------------------------------------------------------------------------
    namespace Signals
    {
        class Signal;
        class SignalsList;

        typedef unsigned int tSignalID;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Contains all the utility classes
    //-----------------------------------------------------------------------------------
    namespace Utils
    {
        class Describable;
        class Path;
        class PropertiesList;
        class StringsMap;
        class StringUtils;
        class StringConverter;
        class Timer;
        class Variant;

        /// @brief Used to represent an ID
        typedef unsigned int tID;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Contains all the core declarations
    //-----------------------------------------------------------------------------------
    namespace Core
    {
        ATHENA_CORE_SYMBOL extern const char* VERSION;
    }
}

#endif
