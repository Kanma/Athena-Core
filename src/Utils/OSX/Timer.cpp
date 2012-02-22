/** @file   Timer.cpp
    @author Philip Abbet

    Implementation of the MacOS X-specific class 'Athena::Utils::Timer'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#include <Athena-Core/Utils/Timer.h>
#include <sys/time.h>

using namespace Athena::Utils;


static const float INV_CLOCKS_PER_MILLISECOND = 1000.0f / (float) CLOCKS_PER_SEC;
static const float INV_CLOCKS_PER_MICROSECOND = 1000000.0f / (float) CLOCKS_PER_SEC;


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

Timer::Timer()
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
    zeroClock = clock();
    gettimeofday(&start, NULL);
}

//-----------------------------------------------------------------------

unsigned long Timer::getMilliseconds()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000;
}

//-----------------------------------------------------------------------

unsigned long Timer::getMicroseconds()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
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
