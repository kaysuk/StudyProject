#pragma once
#include "Train.h"


class PassengerTrain : public Train
{
public:
    PassengerTrain(int id, TimePoint arrivingTime, TimePoint leaveTime);

    std::string ToString() const override;
};
