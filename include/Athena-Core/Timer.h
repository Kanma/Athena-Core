/**	@file	Timer.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Utils::Timer'

    @note   This file only includes a platform-specific header
*/

#ifndef _ATHENA_UTILS_TIMER_H
#define _ATHENA_UTILS_TIMER_H

#include <Athena-Core/Prerequisites.h>

// Bring in the specific platform's header file
#if ATHENA_CORE_PLATFORM == ATHENA_CORE_PLATFORM_WIN32
    #include "WIN32/Timer.h"
#elif ATHENA_CORE_PLATFORM == ATHENA_CORE_PLATFORM_LINUX
    #include "GLX/Timer.h"
#elif ATHENA_CORE_PLATFORM == ATHENA_CORE_PLATFORM_APPLE
    #include "OSX/Timer.h"
#else
    #error Unknown platform
#endif

#endif
