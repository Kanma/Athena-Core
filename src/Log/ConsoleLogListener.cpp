/** @file   ConsoleLogListener.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Log::ConsoleLogListener'
*/

#include <Athena-Core/Log/ConsoleLogListener.h>
#include <Athena-Core/Log/LogManager.h>
#include <string.h>

using namespace Athena::Log;
using namespace std;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char*    __CONTEXT__ = "Console Log listener";


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

ConsoleLogListener::ConsoleLogListener()
{
    // Assertions
    assert(LogManager::getSingletonPtr() && "There isn't any LogManager instance");
}

//-----------------------------------------------------------------------

ConsoleLogListener::~ConsoleLogListener()
{
}


/********************************* METHODS TO IMPLEMENT *********************************/

void ConsoleLogListener::log(const std::string& strTimestamp, tMessageType type,
                             const char* strContext, const std::string& strMessage,
                             const char* strFileName, const char* strFunction,
                             unsigned int uiLine)
{
    // Assertions
    assert(LogManager::getSingletonPtr() && "There isn't any LogManager instance");

    // Write the message to the console
    switch (type)
    {
        case LOG_COMMENT: cout << "(Comment) "; break;
        case LOG_DEBUG:   cout << "(Debug)   "; break;
        case LOG_WARNING: cout << "(Warning) "; break;
        case LOG_ERROR:   cout << "(Error)   "; break;
        case LOG_EVENT:   cout << "(Event)   "; break;
    }

    cout << "[" << strContext << "] " << strMessage << endl;

    if ((type == LOG_ERROR) || (type == LOG_DEBUG))
    {
        if (strlen(strFileName) > 0)
            cout << "        at " << strFileName << ":" << uiLine << endl;

        if (strlen(strFunction) > 0)
            cout << "        in " << strFunction << "(...)" << endl;
    }
}
