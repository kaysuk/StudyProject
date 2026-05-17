#pragma once

#include "TimeTypes.h"

class SimulatedClock
{
public:
    SimulatedClock(TimePoint startTime);

    void setTime(TimePoint currentTime);
    TimePoint now() const;

private:
    TimePoint currentTime;
};
