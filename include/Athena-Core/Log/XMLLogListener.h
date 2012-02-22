/** @file   XMLLogListener.h
    @author Philip Abbet

    Definition of the class 'Athena::Log::XMLLogListener'
*/

#ifndef _ATHENA_LOG_XMLLOGLISTENER_H_
#define _ATHENA_LOG_XMLLOGLISTENER_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Log/ILogListener.h>
#include <fstream>


namespace Athena {
namespace Log {

//----------------------------------------------------------------------------------------
/// @brief    A log listener that save the messages into a XML file
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL XMLLogListener: public ILogListener
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Constructor
    //------------------------------------------------------------------------------------
    XMLLogListener(const std::string& strFileName);

    //------------------------------------------------------------------------------------
    /// @brief    Destructor
    //------------------------------------------------------------------------------------
    virtual ~XMLLogListener();


    //_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Indicates if the file is open
    /// @return 'true' if the file is open
    //------------------------------------------------------------------------------------
    bool isFileOpen() const;


    //_____ Methods to implement __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Log a message in the file
    ///
    /// @param    strTimestamp    Timestamp of the message
    /// @param    type            Type of the message
    /// @param    strContext        Context of the message
    /// @param    strMessage        The message
    /// @param    strFileName        The name of the file in which the call is made
    /// @param    strFunction        The name of the function in which the call is made
    /// @param    uiLine            The line in the file in which the call is made
    //------------------------------------------------------------------------------------
    virtual void log(const std::string& strTimestamp, tMessageType type,
                     const char* strContext, const std::string& strMessage,
                     const char* strFileName, const char* strFunction, unsigned int uiLine);


    //_____ Attributes __________
protected:
    std::ofstream    m_file;        ///< The file
    unsigned long    m_ulID;        ///< Counter for the messages
};

}
}

#endif
