#include "timer.h"

#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <time.h>
#endif

namespace muggle
{
    Timer::Timer()
    {
#if defined(_MSC_VER)
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        ticksPerSecond_ = frequency.QuadPart;
        secondsPerTick_ = 1.0 / ticksPerSecond_;

#endif

        reset();
    }

    Timer::~Timer()
    {
    }

    double Timer::getFrequency() const
    {
        return static_cast<double>(ticksPerSecond_);
    }

    std::uint64_t Timer::getTicks()
    {
#if defined(_MSC_VER)
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return counter.QuadPart;
#endif
    }

    double Timer::getSeconds() const
    {
        return (static_cast<double>(getTicks()) * secondsPerTick_) + startSceondsNegated_ ;
    }

    void Timer::reset()
    {
        startSceondsNegated_ = 0;
        startSceondsNegated_ = -getSeconds();
    }
}