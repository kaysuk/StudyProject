#pragma once
#include "Platform.h"
#include "TimeTypes.h"
#include "Train/Train.h"
#include <memory>
#include <vector>

enum class TrainAssignmentStatus
{
    UNASSIGNED,
    PREDICTED,
    ASSIGNED,
    ARRIVED,
    DELAYED,
    DEPARTED
};

struct TrainAssignment
{
    int trainId = 0;
    int platformId = -1;
    TrainAssignmentStatus status = TrainAssignmentStatus::UNASSIGNED;
    bool wasDelayed = false;
    TimePoint predictedArrival;
    TimeDuration stopTime;
};

struct PlatformAvailability
{
    int platformId = -1;
    TimePoint freeAt{};
};

static const std::chrono::minutes tenMin{10};

class Station
{
private:
    std::vector<std::unique_ptr<Train>> trains;
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<TrainAssignment> assignments;
    TimePoint currentTime;
    int TrainId = 1;
    int platformId = 1;

    void SortTrains(std::vector<std::unique_ptr<Train>>& trains);
    Platform* findFreePlatform();
    Platform* FindPlatformById(int id);
    bool HasAssignment(int trainId) const;

    PlatformAvailability GetFirstPlatformAvailability() const;

public:
    void InformationTable();
    void TrainArriving();
    void AddPassengerTrain(const TimePoint& arrivingTime, const TimePoint& leaveTime);
    void AddPassengerTrain(const TimePoint& arrivingTime, const RealDuration& stopTime);
    void AddPlatform(const std::string& number);

    void ShowTrains() const;




    void UpdateTime(TimePoint now);
};
