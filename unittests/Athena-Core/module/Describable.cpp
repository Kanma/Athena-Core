#include <Athena-Core/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include "../mocks/Describable.h"
#include <v8.h>

using namespace Athena::Utils;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#undef GetObjectPtr1
#define GetObjectPtr1(HANDLE) GetObjectPtr<MockDescribable1>(HANDLE)

#undef GetObjectPtr2
#define GetObjectPtr2(HANDLE) GetObjectPtr<MockDescribable2>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
Handle<Value> MockDescribable1_New(const Arguments& args)
{
    return SetObjectPtr(args.This(), new MockDescribable1());
}


//-----------------------------------------------------------------------

// Constructor
Handle<Value> MockDescribable2_New(const Arguments& args)
{
    return SetObjectPtr(args.This(), new MockDescribable2());
}


/************************************** PROPERTIES *************************************/

Handle<Value> MockDescribable1_GetName(Local<String> property, const AccessorInfo &info)
{
    MockDescribable1* ptr = GetObjectPtr1(info.This());
    assert(ptr);

    return String::New(ptr->strName.c_str());
}

//-----------------------------------------------------------------------

Handle<Value> MockDescribable2_GetIndex(Local<String> property, const AccessorInfo &info)
{
    MockDescribable2* ptr = GetObjectPtr2(info.This());
    assert(ptr);

    return Integer::New(ptr->iIndex);
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Tests_Describable(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();
    
    Handle<FunctionTemplate> mock1 = pManager->getClassTemplate("Tests.MockDescribable1");
    Handle<FunctionTemplate> mock2 = pManager->getClassTemplate("Tests.MockDescribable2");
    
    if (mock1.IsEmpty())
    {
        // Create the object template for MockDescribable1
        mock1 = FunctionTemplate::New(MockDescribable1_New);
        mock1->InstanceTemplate()->SetInternalFieldCount(1);
        mock1->Inherit(pManager->getClassTemplate("Athena.Utils.Describable"));

        // Attributes
        AddAttribute(mock1, "name", MockDescribable1_GetName, 0);

        pManager->declareClassTemplate("Tests.MockDescribable1", mock1);

        // Create the object template for MockDescribable2
        mock2 = FunctionTemplate::New(MockDescribable2_New);
        mock2->InstanceTemplate()->SetInternalFieldCount(1);
        mock2->Inherit(pManager->getClassTemplate("Tests.MockDescribable1"));

        // Attributes
        AddAttribute(mock2, "index", MockDescribable2_GetIndex, 0);

        pManager->declareClassTemplate("Tests.MockDescribable2", mock2);
    }

    // Add the class to the parent
    return parent->Set(String::New("MockDescribable1"), mock1->GetFunction()) &&
           parent->Set(String::New("MockDescribable2"), mock2->GetFunction());
}
