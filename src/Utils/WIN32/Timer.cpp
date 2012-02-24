/** @file   Timer.cpp
    @author Philip Abbet

    Implementation of the Windows-specific class 'Athena::Utils::Timer'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#include <Athena-Core/Utils/Timer.h>

using namespace Athena::Utils;


static const float INV_CLOCKS_PER_MILLISECOND = 1000.0f / (float) CLOCKS_PER_SEC;
static const float INV_CLOCKS_PER_MICROSECOND = 1000000.0f / (float) CLOCKS_PER_SEC;


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

Timer::Timer()
: timerMask(0)
{
    reset();
}

//-----------------------------------------------------------------------

Timer::~Timer()
{
}


/*************************************** METHODS ****************************************/

void Timer::reset()
{
    // Get the current process core mask
    DWORD_PTR procMask;
    DWORD_PTR sysMask;
    GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

    // If procMask is 0, consider there is only one core available
    // (using 0 as procMask will cause an infinite loop below)
    if (procMask == 0)
        procMask = 1;

    // Find the lowest core that this process uses
    if (timerMask == 0)
    {
        timerMask = 1;
        while ((timerMask & procMask) == 0)
            timerMask <<= 1;
    }

    HANDLE thread = GetCurrentThread();

    // Set affinity to the first core
    DWORD oldMask = SetThreadAffinityMask(thread, timerMask);

    // Get the constant frequency
    QueryPerformanceFrequency(&frequency);

    // Query the timer
    QueryPerformanceCounter(&startTime);
    startTick = GetTickCount();

    // Reset affinity
    SetThreadAffinityMask(thread, oldMask);

    lastTime = 0;
    zeroClock = clock();
}

//-----------------------------------------------------------------------

unsigned long Timer::getMilliseconds()
{
    LARGE_INTEGER curTime;

    HANDLE thread = GetCurrentThread();

    // Set affinity to the first core
    DWORD oldMask = SetThreadAffinityMask(thread, timerMask);

    // Query the timer
    QueryPerformanceCounter(&curTime);

    // Reset affinity
    SetThreadAffinityMask(thread, oldMask);

    LONGLONG newTime = curTime.QuadPart - startTime.QuadPart;

    // Scale by 1000 for milliseconds
    unsigned long newTicks = (unsigned long) (1000 * newTime / frequency.QuadPart);

    // Detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - startTick;
    signed long msecOff = (signed long) (newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = std::min(msecOff * frequency.QuadPart / 1000, newTime - lastTime);
        startTime.QuadPart += adjust;
        newTime -= adjust;

        // Re-calculate milliseconds
        newTicks = (unsigned long) (1000 * newTime / frequency.QuadPart);
    }

    // Record last time for adjust
    lastTime = newTime;

    return newTicks;
}

//-----------------------------------------------------------------------

unsigned long Timer::getMicroseconds()
{
    LARGE_INTEGER curTime;

    HANDLE thread = GetCurrentThread();

    // Set affinity to the first core
    DWORD oldMask = SetThreadAffinityMask(thread, timerMask);

    // Query the timer
    QueryPerformanceCounter(&curTime);

    // Reset affinity
    SetThreadAffinityMask(thread, oldMask);

    LONGLONG newTime = curTime.QuadPart - startTime.QuadPart;
    
    // get milliseconds to check against GetTickCount
    unsigned long newTicks = (unsigned long) (1000 * newTime / frequency.QuadPart);
    
    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - startTick;
    signed long msecOff = (signed long)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * frequency.QuadPart / 1000, newTime - lastTime);
        startTime.QuadPart += adjust;
        newTime -= adjust;
    }

    // Record last time for adjust
    lastTime = newTime;

    // scale by 1000000 for microseconds
    unsigned long newMicro = (unsigned long) (1000000 * newTime / frequency.QuadPart);

    return newMicro;
}

//-----------------------------------------------------------------------

unsigned long Timer::getMillisecondsCPU()
{
    clock_t newClock = clock();
    return (unsigned long) ((float) (newClock-zeroClock) * INV_CLOCKS_PER_MILLISECOND);
}

//-----------------------------------------------------------------------

unsigned long Timer::getMicrosecondsCPU()
{
    clock_t newClock = clock();
    return (unsigned long) ((float) (newClock-zeroClock) * INV_CLOCKS_PER_MICROSECOND);
}
