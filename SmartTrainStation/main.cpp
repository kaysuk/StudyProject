#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#include "SimulatedClock.h"
#include "Station/Station.h"

namespace
{
TimePoint MakeTimePoint(int year, unsigned month, unsigned day, int hour, int minute)
{
    return std::chrono::sys_days{std::chrono::year{year} / month / day} +
           std::chrono::hours{hour} + std::chrono::minutes{minute};
}
    RealDuration MakeRealDuration(int hour, int minute, int seconds)
{
    return std::chrono::hours{hour} + std::chrono::minutes{minute} + std::chrono::seconds{seconds};
}
}

// TimeTypes.h
//
// using SystemClock = std::chrono::system_clock;
// using SteadyClock = std::chrono::steady_clock;
//
// using TimePoint = SystemClock::time_point;
// using TimeDuration = SystemClock::duration;
// using RealTimePoint = SteadyClock::time_point;
// using RealDuration = SteadyClock::duration;

int main()
{

    Station station;
    station.AddPlatform("1");
    // station.AddPlatform("2");
    // station.AddPlatform("3");

    const auto startTime = MakeTimePoint(2010, 3, 12, 12, 0);
    const auto arrivalTime = MakeTimePoint(2010, 3, 12, 12, 10);
    const auto departureTime = MakeTimePoint(2010, 3, 12, 12, 20);

    station.AddPassengerTrain(MakeTimePoint(2010, 3, 12, 12, 40), MakeRealDuration(0, 15, 0));
    station.AddPassengerTrain(arrivalTime, departureTime);
    station.AddPassengerTrain(MakeTimePoint(2010, 3, 12, 12, 20), MakeTimePoint(2010, 3, 12, 12, 30));
    station.AddPassengerTrain(MakeTimePoint(2010, 3, 12, 12, 15), MakeRealDuration(0, 10, 30));

    //station.ShowTrains();

    SimulatedClock clock(startTime);

    const RealTimePoint realStart = SteadyClock::now();
    constexpr double speed = 60.0;
    constexpr auto updateInterval = std::chrono::seconds{1};

    while (true) {
        const RealTimePoint nowReal = SteadyClock::now();
        const RealDuration elapsedReal = nowReal - realStart;
        const auto simulatedElapsed =
            std::chrono::duration_cast<TimeDuration>(elapsedReal * speed);
        const TimePoint currentTime = startTime + simulatedElapsed;

        clock.setTime(currentTime);
        station.UpdateTime(clock.now());

        station.TrainArriving();
        station.InformationTable();
        std::this_thread::sleep_for(updateInterval);
    }
}
