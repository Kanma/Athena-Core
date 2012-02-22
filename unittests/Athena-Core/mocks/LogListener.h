/** @file   LogListener.h
    @author Philip Abbet

    Definition of the class 'MockLogListener'
*/

#ifndef _MOCKLOGLISTENER_H_
#define _MOCKLOGLISTENER_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Log/ILogListener.h>


//---------------------------------------------------------------------------------------
/// @brief    Mock implementation of the interface 'Athena::Log::ILogListener'
//---------------------------------------------------------------------------------------
class CMockLogListener: public Athena::Log::ILogListener
{
    //_____ Construction / Destruction __________
public:
    CMockLogListener()
    {
        ++uiInstances;
    }

    virtual ~CMockLogListener()
    {
        --uiInstances;
    }


    //_____ Methods to implement __________
public:
    virtual void log(const std::string& strTimestamp, Athena::Log::tMessageType type,
                     const char* strContext, const std::string& strMessage,
                     const char* strFileName, const char* strFunction, unsigned int uiLine)
    {
        this->strTimestamp    = strTimestamp;
        this->type            = type;
        this->strContext    = strContext;
        this->strMessage    = strMessage;
        this->strFileName    = strFileName;
        this->strFunction    = strFunction;
        this->uiLine        = uiLine;
    }


    //_____ Attributes __________
public:
    std::string                    strTimestamp;
    Athena::Log::tMessageType    type;
    std::string                    strContext;
    std::string                    strMessage;
    std::string                    strFileName;
    std::string                    strFunction;
    unsigned int                uiLine;

    static unsigned int uiInstances;
};

#endif
