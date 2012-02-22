/** @file   LogManager.h
    @author Philip Abbet

    Declaration of the class 'Athena::Log::LogManager'
*/

#ifndef _ATHENA_LOG_LOGMANAGER_H_
#define _ATHENA_LOG_LOGMANAGER_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Log/Declarations.h>
#include <Athena-Core/Utils/Iterators.h>
#include <iostream>
#include <time.h>


namespace Athena {
namespace Log {

//----------------------------------------------------------------------------------------
/// @brief    This class is used to log messages
///
/// The messages are sent to the registered listeners
///
/// @remark This class is a singleton.
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL LogManager: public Utils::Singleton<LogManager>
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Constructor
    //------------------------------------------------------------------------------------
    LogManager();

    //------------------------------------------------------------------------------------
    /// @brief    Destructor
    //------------------------------------------------------------------------------------
    ~LogManager();

    //------------------------------------------------------------------------------------
    /// @brief    Returns a reference to the instance of the log manager
    /// @return    A reference to the manager
    //------------------------------------------------------------------------------------
    static LogManager& getSingleton();

    //------------------------------------------------------------------------------------
    /// @brief    Returns a pointer to the instance of the log manager
    /// @return    A pointer to the manager
    //------------------------------------------------------------------------------------
    static LogManager* getSingletonPtr();


    //_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Add a listener to the list
    ///
    ///    @param    pListener            The listener
    ///    @param    bManageDestruction    Indicates if the manager can destroy the listener or
    ///                             not
    //------------------------------------------------------------------------------------
    void addListener(ILogListener* pListener, bool bManageDestruction = false);

    //------------------------------------------------------------------------------------
    /// @brief    Remove a listener from the list
    ///
    ///    @param    pListener    The listener
    //------------------------------------------------------------------------------------
    void removeListener(ILogListener* pListener);

    //------------------------------------------------------------------------------------
    /// @brief    Log a message in the file
    ///
    /// @param    type            Type of the message
    /// @param    strContext        Context of the message
    /// @param    strMessage        The message
    /// @param    strFileName        The name of the file in which the call is made
    /// @param    strFunction        The name of the function in which the call is made
    /// @param    uiLine            The line in the file in which the call is made
    //------------------------------------------------------------------------------------
    void log(tMessageType type, const char* strContext, const std::string& strMessage,
             const char* strFileName, const char* strFunction, unsigned int uiLine);

    //------------------------------------------------------------------------------------
    /// @brief    Log a message in the file
    ///
    /// @param    type            Type of the message
    /// @param    strContext        Context of the message
    /// @param    strMessage        The message
    /// @param    strFileName        The name of the file in which the call is made
    /// @param    strFunction        The name of the function in which the call is made
    /// @param    uiLine            The line in the file in which the call is made
    //------------------------------------------------------------------------------------
    inline static void Log(tMessageType type, const char* strContext,
                           const std::string& strMessage, const char* strFileName,
                           const char* strFunction, unsigned int uiLine)
    {
        if (ms_Singleton)
        {
            ms_Singleton->log(type, strContext, strMessage, strFileName,
                              strFunction, uiLine);
        }

        // We want to be notified about errors (at least during development)
        else if (type == LOG_ERROR)
        {
            std::cerr << "ERROR: " << strMessage.c_str() << std::endl;
        }
    }


    //_____ Internal types __________
private:
    struct tListener
    {
        ILogListener*    pListener;
        bool            bManageDestruction;
    };

    typedef std::vector<tListener>                  tListenersList;
    typedef Utils::VectorIterator<tListenersList>   tListenersIterator;
    typedef tListenersList::iterator                tListenersNativeIterator;


    //_____ Attributes __________
private:
    tListenersList  m_listeners;        ///< The listeners
    time_t            m_startTimestamp;    ///< Timestamp at which the log manager was created
};

}
}

#endif
