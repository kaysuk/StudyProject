#pragma once
#include <string>
#include "TimeTypes.h"
#include <format>
#include <sstream>

enum class TrainStatus
{
    ON_TIME,
    DEPARTED,
    DELAYED
};



class Train
{
private:
    int id;
    std::string number;
    TimePoint arrivingTime;
    TimePoint leaveTime;
    TimeDuration stopTime;
    TrainStatus status = TrainStatus::ON_TIME;

    void AddStopTime(const TimePoint& arriving, const TimePoint& leave);
public:
    Train(int id, TimePoint arrivingTime, TimePoint leaveTime);
    const int getId() const { return id; };
    const std::string& getNumber() const { return number; };
    TimePoint getArrivingTime() const { return arrivingTime; }
    TimePoint getLeaveTime() const { return leaveTime; };
    TimeDuration getStopTime() const { return stopTime; }

    virtual std::string ToString() const = 0;
    virtual ~Train() = default;
};
