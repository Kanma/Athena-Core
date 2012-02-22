/** @file   Scripting.h
    @author Philip Abbet

    Scripting-related declarations for the Athena-Core module
*/

#ifndef _ATHENA_CORE_SCRIPTING_H_
#define _ATHENA_CORE_SCRIPTING_H_

#include <v8.h>
#include <Athena-Core/Utils/Variant.h>
#include <Athena-Core/Utils/PropertiesList.h>


namespace Athena {
namespace Utils {

    v8::Handle<v8::Value> toJS(Variant* pValue);

    v8::Handle<v8::Value> toJS(PropertiesList::tPropertiesIterator propIter);

    Variant* fromJS(v8::Handle<v8::Value> value);

}
}

#endif
