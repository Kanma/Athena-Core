/** @file   Athena_Utils_Timer.h
    @author Philip Abbet

    Declaration of the Windows-specific class 'Athena::Utils::Timer'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#ifndef _ATHENA_UTILS_WIN32_TIMER_H
#define _ATHENA_UTILS_WIN32_TIMER_H

#include <Athena-Core/Prerequisites.h>

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <windows.h>
#include <time.h>


namespace Athena {
namespace Utils {

//----------------------------------------------------------------------------------------
/// @brief  Represents a timer
//----------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL Timer
{
    //_____ Construction / Destruction __________
public:
    Timer();
    ~Timer();


    //_____ Management of the timer __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Resets the timer
    //------------------------------------------------------------------------------------
    void reset();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of milliseconds elapsed since the last reset
    /// @return The number of milliseconds
    //------------------------------------------------------------------------------------
    unsigned long getMilliseconds();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of microseconds elapsed since the last reset
    /// @return The number of microseconds
    //------------------------------------------------------------------------------------
    unsigned long getMicroseconds();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of milliseconds elapsed since the last reset, only CPU
    ///         time measured
    /// @return The number of milliseconds
    //------------------------------------------------------------------------------------
    unsigned long getMillisecondsCPU();

    //------------------------------------------------------------------------------------
    /// @brief  Returns the number of microseconds elapsed since the last reset, only CPU
    ///         time measured
    /// @return The number of milliseconds
    //------------------------------------------------------------------------------------
    unsigned long getMicrosecondsCPU();


    //_____ Attributes __________
private:
    clock_t         zeroClock;
    LARGE_INTEGER   startTime;
    LARGE_INTEGER   frequency;
    DWORD           startTick;
    LONGLONG        lastTime;
    DWORD_PTR       timerMask;
};

}
}

#endif
