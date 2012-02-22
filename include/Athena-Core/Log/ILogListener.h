/** @file   ILogListener.h
    @author Philip Abbet

    Definition of the class 'Athena::Log::ILogListener'
*/

#ifndef _ATHENA_LOG_ILOGLISTENER_H_
#define _ATHENA_LOG_ILOGLISTENER_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Log/Declarations.h>


namespace Athena {
namespace Log {

//---------------------------------------------------------------------------------------
/// @brief    Interface for a log listener
///
/// The log listeners must be registered with the Log Manager
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL ILogListener
{
    //_____ Construction / Destruction __________
public:
    ILogListener() {};
    virtual ~ILogListener() {};


    //_____ Methods to implement __________
public:
    //---------------------------------------------------------------------------------------
    /// @brief    Called to log a message
    /// @param    strTimestamp    Timestamp of the message
    /// @param    type            Type of the message
    /// @param    strContext        Context of the message
    /// @param    strMessage        The message
    /// @param    strFileName        The name of the file in which the call is made
    /// @param    strFunction        The name of the function in which the call is made
    /// @param    uiLine            The line in the file in which the call is made
    //---------------------------------------------------------------------------------------
    virtual void log(const std::string& strTimestamp, tMessageType type, const char* strContext,
                     const std::string& strMessage, const char* strFileName,
                     const char* strFunction, unsigned int uiLine) = 0;
};

}
}

#endif
