/** @file   Describable.cpp
    @author Philip Abbet
    
    Javascript binding of the class Athena::Utils::Describable
*/

#include <Athena-Core/Utils/Describable.h>
#include <Athena-Core/Scripting.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Color.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Utils;
using namespace Athena::Math;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetObjectPtr(HANDLE) GetObjectPtr<Describable>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

//-----------------------------------------------------------------------

// Constructor
Handle<Value> Describable_New(const Arguments& args)
{
    return SetObjectPtr(args.This(), new Describable());
}


/**************************************** METHODS ***************************************/

Handle<Value> Describable_GetProperties(const Arguments& args)
{
    Describable* self = GetObjectPtr(args.This());
    assert(self);


    Handle<Object> jsProperties = Object::New();

    PropertiesList* pProperties = self->getProperties();
	PropertiesList::tCategoriesIterator catIter = pProperties->getCategoriesIterator();
    while (catIter.hasMoreElements())
    {
        PropertiesList::tCategory* category = catIter.peekNextPtr();

        Handle<Object> jsCategory = Object::New();
        
    	PropertiesList::tPropertiesIterator propIter(category->values.begin(),
    	                                             category->values.end());
        while (propIter.hasMoreElements())
        {
            PropertiesList::tProperty* property = propIter.peekNextPtr();

            Handle<Value> value;
            
            switch (property->pValue->getType())
            {
                case Variant::STRING:
                    value = String::New(property->pValue->toString().c_str());
                    break;
                
                case Variant::INTEGER:
                case Variant::SHORT:
                case Variant::CHAR:
                    value = Int32::New(property->pValue->toInt());
                    break;
                
                case Variant::UNSIGNED_INTEGER:
                case Variant::UNSIGNED_SHORT:
                case Variant::UNSIGNED_CHAR:
                    value = Uint32::New(property->pValue->toUInt());
                    break;

                case Variant::FLOAT:
                case Variant::DOUBLE:
                    value = Number::New(property->pValue->toFloat());
                    break;
                
                case Variant::BOOLEAN:
                    value = Boolean::New(property->pValue->toBool());
                    break;

                case Variant::VECTOR3:
                {
                    Vector3 v = property->pValue->toVector3();

                    Handle<Object> obj = Object::New();
                    obj->Set(String::New("x"), Number::New(v.x));
                    obj->Set(String::New("y"), Number::New(v.y));
                    obj->Set(String::New("z"), Number::New(v.z));

                    value = obj;
                    break;
                }

                case Variant::QUATERNION:
                {
                    Quaternion q = property->pValue->toQuaternion();

                    Handle<Object> obj = Object::New();
                    obj->Set(String::New("w"), Number::New(q.w));
                    obj->Set(String::New("x"), Number::New(q.x));
                    obj->Set(String::New("y"), Number::New(q.y));
                    obj->Set(String::New("z"), Number::New(q.z));

                    value = obj;
                    break;
                }

                case Variant::COLOR:
                {
                    Color c = property->pValue->toColor();

                    Handle<Object> obj = Object::New();
                    obj->Set(String::New("r"), Number::New(c.r));
                    obj->Set(String::New("g"), Number::New(c.g));
                    obj->Set(String::New("b"), Number::New(c.b));
                    obj->Set(String::New("a"), Number::New(c.a));

                    value = obj;
                    break;
                }

                case Variant::RADIAN:
                case Variant::DEGREE:
                    value = Number::New(property->pValue->toRadian().valueRadians());
                    break;
                
                case Variant::STRUCT:
                {
                    // Variant::tFieldsIterator iter = property->pValue->getFieldsIterator();
                    break;
                }
            }
            
            jsCategory->Set(String::New(property->strName.c_str()), value);

            propIter.moveNext();
        }
        
        jsProperties->Set(String::New(category->strName.c_str()), jsCategory);

        catIter.moveNext();
    }

    return jsProperties;
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Utils_Describable(Handle<Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();
    
    Handle<FunctionTemplate> describable = pManager->getClassTemplate("Athena.Utils.Describable");
    
    if (describable.IsEmpty())
    {
        // Declaration of the class
        describable = FunctionTemplate::New(Describable_New);
        describable->InstanceTemplate()->SetInternalFieldCount(1);
        
        // Methods
        AddMethod(describable, "properties", Describable_GetProperties);

        pManager->declareClassTemplate("Athena.Utils.Describable", describable);
    }

    // Add the class to the parent
    return parent->Set(String::New("Describable"), describable->GetFunction());
}
