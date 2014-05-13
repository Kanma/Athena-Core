#include <UnitTest++.h>
#include <Athena-Scripting/ScriptingManager.h>
#include "../environments/ScriptingTestEnvironment.h"
#include <iostream>


using namespace Athena::Scripting;
using namespace Athena::Utils;
using namespace v8;


#define JS_TEST(NAME)                                   \
    TEST_FIXTURE(ScriptingTestEnvironment, NAME)                \
    {                                                           \
        HandleScope handle_scope;                               \
                                                                \
        Handle<Value> result = pScriptingManager->executeFile(  \
                ATHENA_CORE_UNITTESTS_SCRIPTS_PATH #NAME ".js"); \
                                                                \
        CHECK(!result.IsEmpty());                               \
        CHECK(pScriptingManager->getLastError().empty());       \
    }



SUITE(Scripting_Utils_Describable)
{
    JS_TEST(Describable_PropertiesSerialization);
    JS_TEST(Describable_GetProperties);
    JS_TEST(Describable_SetProperties);
    JS_TEST(Describable_UnknownProperties);
    JS_TEST(Describable_DelayedProperties);
}


SUITE(Scripting_Utils_PropertiesList)
{
    JS_TEST(PropertiesList_SetGetValueWithCategory);
    JS_TEST(PropertiesList_SetGetValueWithoutCategory);
    JS_TEST(PropertiesList_GetValueFromDefaultCategory);
    JS_TEST(PropertiesList_GetValueFromSelectedCategory);
    JS_TEST(PropertiesList_ModifyValue);
    JS_TEST(PropertiesList_SelectCategories);
    JS_TEST(PropertiesList_InsertCategoryAtBeginning);
    JS_TEST(PropertiesList_ValuesEnumeration);
    JS_TEST(PropertiesList_ValuesEnumerationInSelectedCategory);
    JS_TEST(PropertiesList_ValuesEnumerationInNonExistingCategory);
    JS_TEST(PropertiesList_AppendList);
    JS_TEST(PropertiesList_PrependList);
}


SUITE(Scripting_Signals_Signal)
{
    JS_TEST(Signal_ConnectDisconnectFunctionSlot);
    JS_TEST(Signal_FunctionSlot);
    JS_TEST(Signal_TwoFunctionSlots);
    JS_TEST(Signal_ConnectOnlyOnceFunctionSlot);
    JS_TEST(Signal_ConnectDisconnectMethodSlot);
    JS_TEST(Signal_MethodSlot);
    JS_TEST(Signal_TwoMethodSlots);
    JS_TEST(Signal_ConnectOnlyOneMethodSlot);
}


SUITE(Scripting_Signals_SignalsList)
{
    JS_TEST(SignalsList_ConnectDisconnectFunctionSlot);
    JS_TEST(SignalsList_FunctionSlot);
    JS_TEST(SignalsList_TwoFunctionSlots);
    JS_TEST(SignalsList_ConnectOnlyOnceFunctionSlot);
    JS_TEST(SignalsList_ConnectDisconnectMethodSlot);
    JS_TEST(SignalsList_MethodSlot);
    JS_TEST(SignalsList_TwoMethodSlots);
    JS_TEST(SignalsList_ConnectOnlyOneMethodSlot);
    JS_TEST(SignalsList_TwoSignals);
}


SUITE(Scripting_Signals_SignalsUtils)
{
    JS_TEST(SignalsUtils_CreateSignalID);
    JS_TEST(SignalsUtils_RetrieveSignalID);
    JS_TEST(SignalsUtils_RetrieveSignalName);
    JS_TEST(SignalsUtils_RetrieveInvalidSignalName);
}
