#ifndef _SCRIPTINGTESTENVIRONMENT_H_
#define _SCRIPTINGTESTENVIRONMENT_H_

#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>


extern bool init_test_module(v8::Handle<v8::Context> context);


struct ScriptingTestEnvironment
{
    Athena::Scripting::ScriptingManager* pScriptingManager;

	ScriptingTestEnvironment()
	: pScriptingManager(0)
	{
        v8::HandleScope handle_scope;

        pScriptingManager = new Athena::Scripting::ScriptingManager();
        
        pScriptingManager->import("Athena.Core");

        init_test_module(pScriptingManager->mainContext());

    	pScriptingManager->executeFile(ATHENA_SCRIPTING_JS_TEST_RUNNER_PATH "tests.js");
	}

	~ScriptingTestEnvironment()
	{
        delete pScriptingManager;
	}
};


#endif
