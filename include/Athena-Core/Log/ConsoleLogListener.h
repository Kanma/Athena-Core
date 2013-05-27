/** @file   ConsoleLogListener.h
    @author Philip Abbet

    Definition of the class 'Athena::Log::ConsoleLogListener'
*/

#ifndef _ATHENA_LOG_CONSOLELOGLISTENER_H_
#define _ATHENA_LOG_CONSOLELOGLISTENER_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Log/ILogListener.h>
#include <fstream>


namespace Athena {
namespace Log {

//----------------------------------------------------------------------------------------
/// @brief  A log listener that output messages to the console
//----------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL ConsoleLogListener: public ILogListener
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    //------------------------------------------------------------------------------------
    ConsoleLogListener();

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    virtual ~ConsoleLogListener();


    //_____ Methods to implement __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Log a message to the console
    ///
    /// @param  strTimestamp    Timestamp of the message
    /// @param  type            Type of the message
    /// @param  strContext      Context of the message
    /// @param  strMessage      The message
    /// @param  strFileName     The name of the file in which the call is made
    /// @param  strFunction     The name of the function in which the call is made
    /// @param  uiLine          The line in the file in which the call is made
    //------------------------------------------------------------------------------------
    virtual void log(const std::string& strTimestamp, tMessageType type,
                     const char* strContext, const std::string& strMessage,
                     const char* strFileName, const char* strFunction, unsigned int uiLine);
                 };

}
}

#endif
