/** @file   Scripting.h
    @author Philip Abbet

    Scripting-related declarations for the Athena-Core module
*/

#ifndef _ATHENA_CORE_SCRIPTING_H_
#define _ATHENA_CORE_SCRIPTING_H_

#include <v8.h>

namespace Athena {

namespace Utils {

    enum
    {
        CLASSID_DESCRIBABLE = 200,
    };

    extern v8::Persistent<v8::FunctionTemplate> function_Utils_Describable;

}

}

#endif
