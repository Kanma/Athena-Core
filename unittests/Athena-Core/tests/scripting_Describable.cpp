#include <UnitTest++.h>
#include <Athena-Scripting/ScriptingManager.h>
#include "../environments/ScriptingTestEnvironment.h"
#include <iostream>


using namespace Athena::Scripting;
using namespace Athena::Utils;
using namespace v8;


SUITE(Scripting_Describable)
{
	TEST_FIXTURE(ScriptingTestEnvironment, TestFixture)
	{
        HandleScope handle_scope;

        Handle<Value> result = pScriptingManager->executeFile(ATHENA_CORE_UNITTESTS_SCRIPTS_PATH "Describable_CategoriesOrder.js");

        CHECK(!result.IsEmpty());
        CHECK(pScriptingManager->getLastError().empty());
	}
}
