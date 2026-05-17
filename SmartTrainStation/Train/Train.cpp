#include "Train.h"

Train::Train(int id, TimePoint arrivingTime, TimePoint leaveTime) :
    id(id), arrivingTime(arrivingTime), leaveTime(leaveTime) { AddStopTime(arrivingTime, leaveTime); }

void Train::AddStopTime(const TimePoint& arriving, const TimePoint& leave) { stopTime = leave - arriving; }
