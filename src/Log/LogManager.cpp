/** @file   LogManager.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Log::LogManager'
*/

#include <Athena-Core/Log/LogManager.h>
#include <Athena-Core/Log/ILogListener.h>
#include <Athena-Core/Utils/StringConverter.h>

using namespace Athena::Log;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS **************************************/

/// Context used for logging
static const char*    __CONTEXT__ = "Log manager";

template<> LogManager* Singleton<LogManager>::ms_Singleton = 0;


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

LogManager::LogManager()
: m_startTimestamp(0)
{
    m_startTimestamp = time(0);
}

//-----------------------------------------------------------------------

LogManager::~LogManager()
{
    while (m_listeners.size() > 0)
    {
        if (m_listeners.front().bManageDestruction)
                delete m_listeners.front().pListener;

        m_listeners.erase(m_listeners.begin());
    }
}

//-----------------------------------------------------------------------

LogManager& LogManager::getSingleton()
{
    return Singleton<LogManager>::getSingleton();
}

//-----------------------------------------------------------------------

LogManager* LogManager::getSingletonPtr()
{
    return Singleton<LogManager>::getSingletonPtr();
}


/*************************************** METHODS ****************************************/

void LogManager::addListener(ILogListener* pListener, bool bManageDestruction)
{
    // Assertions
    assert(getSingletonPtr());
    assert(pListener);

    tListener listener;
    listener.pListener            = pListener;
    listener.bManageDestruction    = bManageDestruction;

    m_listeners.push_back(listener);
}

//-----------------------------------------------------------------------

void LogManager::removeListener(ILogListener* pListener)
{
    // Assertions
    assert(getSingletonPtr());
    assert(pListener);

    tListenersNativeIterator iter, iterEnd;
    for (iter = m_listeners.begin(), iterEnd = m_listeners.end(); iter != iterEnd; ++iter)
    {
        if (iter->pListener == pListener)
        {
            if (iter->bManageDestruction)
                delete iter->pListener;

            m_listeners.erase(iter);
            return;
        }
    }
}

//-----------------------------------------------------------------------

void LogManager::log(tMessageType type, const char* strContext, const std::string& strMessage,
                     const char* strFileName, const char* strFunction, unsigned int uiLine)
{
    // Assertions
    assert(getSingletonPtr());

    // Optimisation
    if (!m_listeners.empty())
    {
        // Compute the timestamp
        div_t result = div((int) (time(0) - m_startTimestamp), 60);
        div_t result2 = div(result.quot, 60);

        string strTimestamp = StringConverter::toString(result.quot) + "m " +
                              StringConverter::toString(result.rem) + "s ";

        if (result2.quot > 0)
            strTimestamp = StringConverter::toString(result2.quot) + "h " + strTimestamp;

        // Log into all the registered listeners
        tListenersIterator iter(m_listeners);
        while (iter.hasMoreElements())
        {
            iter.getNext().pListener->log(strTimestamp, type, strContext, strMessage,
                                          strFileName, strFunction, uiLine);
        }
    }
}
