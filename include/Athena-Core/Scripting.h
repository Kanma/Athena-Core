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

    namespace Signals {

        // The conversion functions are implemented in Athena-Scripting (include
        // Athena-Scripting/Conversions.h)
    }

    namespace Utils {

        // Some conversion functions are implemented in Athena-Scripting (include
        // Athena-Scripting/Conversions.h)

        ATHENA_CORE_SYMBOL v8::Handle<v8::Value> toJavaScript(Variant* pValue);

        ATHENA_CORE_SYMBOL v8::Handle<v8::Value> toJavaScript(PropertiesList::tPropertiesIterator propIter);

        ATHENA_CORE_SYMBOL Variant* fromJSVariant(v8::Handle<v8::Value> value);

    }
}

#endif
