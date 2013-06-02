#include <UnitTest++.h>
#include <Athena-Core/Log/LogManager.h>
#include "../mocks/LogListener.h"

using namespace Athena;
using namespace Athena::Log;


unsigned int CMockLogListener::uiInstances = 0;


TEST(LogManager_Singleton)
{
    CHECK(!LogManager::getSingletonPtr());

    LogManager* pLogManager = new LogManager();
    CHECK(pLogManager == LogManager::getSingletonPtr());

    delete pLogManager;
    CHECK(!LogManager::getSingletonPtr());
}


struct LogEnvironment
{
    LogEnvironment()
    {
        pLogManager = new LogManager();
    }

    ~LogEnvironment()
    {
        delete pLogManager;
    }

    LogManager* pLogManager;
};


SUITE(LogManager_Listener)
{
    TEST_FIXTURE(LogEnvironment, OneListener)
    {
        CMockLogListener listener;

        pLogManager->addListener(&listener);

        pLogManager->log(LOG_EVENT, "Test: OneListener", "This is a message",
                      __FILE__, __FUNCTION__, 1234);

        CHECK_EQUAL(LOG_EVENT,              listener.type);
        CHECK_EQUAL("Test: OneListener",    listener.strContext);
        CHECK_EQUAL("This is a message",    listener.strMessage);
        CHECK_EQUAL(__FILE__,               listener.strFileName);
        CHECK_EQUAL(__FUNCTION__,           listener.strFunction);
        CHECK_EQUAL(1234,                   listener.uiLine);

        pLogManager->removeListener(&listener);
    }


    TEST_FIXTURE(LogEnvironment, TwoListeners)
    {
        CMockLogListener listener1;
        CMockLogListener listener2;

        pLogManager->addListener(&listener1);
        pLogManager->addListener(&listener2);

        pLogManager->log(LOG_EVENT, "Test: TwoListeners", "This is a message",
                         __FILE__, __FUNCTION__, 1234);

        CHECK_EQUAL(listener1.strTimestamp, listener2.strTimestamp);
        CHECK_EQUAL(listener1.type,         listener2.type);
        CHECK_EQUAL(listener1.strContext,   listener2.strContext);
        CHECK_EQUAL(listener1.strMessage,   listener2.strMessage);
        CHECK_EQUAL(listener1.strFileName,  listener2.strFileName);
        CHECK_EQUAL(listener1.strFunction,  listener2.strFunction);
        CHECK_EQUAL(listener1.uiLine,       listener2.uiLine);

        pLogManager->removeListener(&listener1);
        pLogManager->removeListener(&listener2);
    }


    TEST_FIXTURE(LogEnvironment, NoDestructionManagement)
    {
        CMockLogListener listener;

        pLogManager->addListener(&listener);
        CHECK_EQUAL(1, CMockLogListener::uiInstances);

        pLogManager->removeListener(&listener);
        CHECK_EQUAL(1, CMockLogListener::uiInstances);
    }


    TEST_FIXTURE(LogEnvironment, NoDestructionManagement2)
    {
        CMockLogListener listener;

        pLogManager->addListener(&listener);
        CHECK_EQUAL(1, CMockLogListener::uiInstances);

        delete pLogManager;
        pLogManager = 0;

        CHECK_EQUAL(1, CMockLogListener::uiInstances);
    }


    TEST_FIXTURE(LogEnvironment, DestructionManagement)
    {
        CMockLogListener* pListener = new CMockLogListener();

        pLogManager->addListener(pListener, true);
        CHECK_EQUAL(1, CMockLogListener::uiInstances);

        pLogManager->removeListener(pListener);
        CHECK_EQUAL(0, CMockLogListener::uiInstances);
    }


    TEST_FIXTURE(LogEnvironment, DestructionManagement2)
    {
        CMockLogListener* pListener = new CMockLogListener();

        pLogManager->addListener(pListener, true);
        CHECK_EQUAL(1, CMockLogListener::uiInstances);

        delete pLogManager;
        pLogManager = 0;

        CHECK_EQUAL(0, CMockLogListener::uiInstances);
    }
}
