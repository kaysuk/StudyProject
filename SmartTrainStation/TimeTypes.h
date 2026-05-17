#pragma once

#include <chrono>

using SystemClock = std::chrono::system_clock;
using SteadyClock = std::chrono::steady_clock;

using TimePoint = SystemClock::time_point;
using TimeDuration = SystemClock::duration;
using RealTimePoint = SteadyClock::time_point;
using RealDuration = SteadyClock::duration;
