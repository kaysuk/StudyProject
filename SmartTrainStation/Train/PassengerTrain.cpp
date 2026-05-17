#include "PassengerTrain.h"

PassengerTrain::PassengerTrain(int id, TimePoint arrivingTime, TimePoint leaveTime) :
    Train(id, arrivingTime, leaveTime) {}

std::string PassengerTrain::ToString() const
{
    std::ostringstream out;
    out << "[" << getId() << "] " << getNumber() << " arrives at: " << std::format("{:%H:%M}", getArrivingTime())
        << " and leave: " << std::format("{:%H:%M}", getLeaveTime())
        << "\nStop duration: " << std::format("{:%H:%M}", getStopTime());
    return out.str();
}