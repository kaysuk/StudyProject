#include "SimulatedClock.h"

SimulatedClock::SimulatedClock(TimePoint startTime)
    : currentTime(startTime) {}

void SimulatedClock::setTime(TimePoint newTime)
{
    currentTime = newTime;
}

TimePoint SimulatedClock::now() const
{
    return currentTime;
}
