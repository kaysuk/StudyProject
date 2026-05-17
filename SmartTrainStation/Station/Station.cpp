#include "Station.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "Train/PassengerTrain.h"

void Station::InformationTable()
{
    system("clear");
    std::cout << "\n=== Platform Board ===\n" << std::flush;
    std::cout << "Time: " << std::format("{:%H:%M}", currentTime) << "\n";
    std::cout << "Train |    Arriving    | Platform |  Status   \n";
    std::cout << "--------------------------------------------------\n";
    for (size_t i = 0; i < trains.size(); ++i)
    {
        auto it = std::find_if(assignments.begin(), assignments.end(),
            [&](const TrainAssignment& a) { return a.trainId == trains[i]->getId(); });
        if (it->predictedArrival != trains[i]->getArrivingTime() && it->status == TrainAssignmentStatus::DELAYED)
            std::cout << " " << trains[i]->getId() << "    | " << std::format("{:%H:%M}", trains[i]->getArrivingTime()) <<
                std::format(" -> {:%H:%M}", it->predictedArrival) << " |";
        else
            std::cout << " " << trains[i]->getId() << "    |     " << std::format("{:%H:%M}", trains[i]->getArrivingTime()) << "      |";
        if (it == assignments.end())
        {
            std::cout << "    --    |  Waiting...\n";
            continue;
        }

        const TrainAssignment& assgn = *it;
        if (assgn.status == TrainAssignmentStatus::ASSIGNED || assgn.status == TrainAssignmentStatus::ARRIVED ||
            assgn.status == TrainAssignmentStatus::DELAYED)
            std::cout << "    " << assgn.platformId << "   ";
        else
            std::cout << "   --   ";

        std::string st;
        switch (assgn.status)
        {
        default: st = "Waiting..."; break;
        case TrainAssignmentStatus::PREDICTED:
            st = std::format("Predicted {:%H:%M}", assgn.predictedArrival); break;
        case TrainAssignmentStatus::ASSIGNED:
            st = std::format("Platform {}", assgn.platformId); break;
        case TrainAssignmentStatus::ARRIVED:
            st = std::format("Arrived at {}", assgn.platformId); break;
        case TrainAssignmentStatus::DEPARTED: st = "Processed"; break;
        case TrainAssignmentStatus::DELAYED:
            st = std::format("[DELAYED] at {:%H:%M}", assgn.predictedArrival); break;
        }
        std::cout << "  |  " << st << "\n" << std::flush;
    }
    std::cout << "===================================================\n";
}

void Station::SortTrains(std::vector<std::unique_ptr<Train>>& trains)
{
    std::sort(trains.begin(), trains.end(),
        [](const std::unique_ptr<Train>& lhs, const std::unique_ptr<Train>&rhs)
        { return lhs->getArrivingTime() < rhs->getArrivingTime();});
}

Platform* Station::findFreePlatform()
{
    for (auto& p : platforms)
    {
        if (p->getStatusPlatform()) { return p.get(); }
    }
    return nullptr;
}

Platform* Station::FindPlatformById(int id)
{
    for (auto& p : platforms)
    {
        if (p->getPlatformId() == id)
            return p.get();
    }
    return nullptr;
}

// Вернет True если TrainId еть в assignments
bool Station::HasAssignment(int trainId) const
{
    return std::any_of(assignments.begin(), assignments.end(),
        [trainId](const TrainAssignment& a) { return a.trainId == trainId; });
}

void Station::TrainArriving()
{
    std::vector<int> departedTrainIds;
    for (auto& t : trains)
    {
        if (t->getArrivingTime() >= currentTime &&
            t->getArrivingTime() <= currentTime + tenMin &&
            !HasAssignment(t->getId()))
        { assignments.push_back(TrainAssignment{t->getId(), -1, TrainAssignmentStatus::PREDICTED, false, t->getArrivingTime(), t->getStopTime()}); }
    }

    if (!assignments.empty())
    {
        for (auto& a : assignments)
        {
            switch (a.status)
            {
                case TrainAssignmentStatus::ASSIGNED:
                    if (a.predictedArrival <= currentTime)
                    {
                        a.status = TrainAssignmentStatus::ARRIVED;
                    }
                    break;
                case TrainAssignmentStatus::PREDICTED:
                {
                    if (Platform* freePlatform = findFreePlatform(); freePlatform != nullptr)
                    {
                        a.platformId = freePlatform->getPlatformId();
                        a.status = TrainAssignmentStatus::ASSIGNED;
                        freePlatform->setStatusPlatform(false);
                        break;
                    }

                    const PlatformAvailability availability = GetFirstPlatformAvailability();
                    if (availability.platformId == -1)
                    {
                        break;
                    }

                    if (a.predictedArrival <= availability.freeAt)
                    {
                        a.status = TrainAssignmentStatus::DELAYED;
                        a.wasDelayed = true;
                        a.platformId = availability.platformId;
                        a.predictedArrival = availability.freeAt;
                    }
                    break;
                }
                case TrainAssignmentStatus::DELAYED:
                    if (a.predictedArrival <= currentTime)
                    {
                        if (Platform* delayedPlatform = FindPlatformById(a.platformId);
                            delayedPlatform != nullptr && delayedPlatform->getStatusPlatform())
                        {
                            a.status = TrainAssignmentStatus::ARRIVED;
                            delayedPlatform->setStatusPlatform(false);
                        }
                    }
                    break;
                case TrainAssignmentStatus::ARRIVED:
                    if (a.predictedArrival + a.stopTime <= currentTime)
                    { a.status = TrainAssignmentStatus::DEPARTED; }
                    break;
                case TrainAssignmentStatus::DEPARTED:
                    if (Platform* p = FindPlatformById(a.platformId))
                        p->setStatusPlatform(true);
                    if (std::find(departedTrainIds.begin(), departedTrainIds.end(), a.trainId) == departedTrainIds.end())
                        departedTrainIds.push_back(a.trainId);
                    break;
                default:
                    break;
            }
        }
    }
    if (!departedTrainIds.empty())
    {
        for (int id : departedTrainIds)
        {
            auto itTrain = std::remove_if(trains.begin(), trains.end(),
                [id](const std::unique_ptr<Train>& t) { return t->getId() == id; });
            trains.erase(itTrain, trains.end());

            auto itAssignment = std::find_if(assignments.begin(), assignments.end(),
                [id](const TrainAssignment& a) { return a.trainId == id; });
            if (itAssignment != assignments.end())
            {
                if (Platform* p = FindPlatformById(itAssignment->platformId))
                    p->setStatusPlatform(true);
                assignments.erase(itAssignment);
            }
        }
    }
}

PlatformAvailability Station::GetFirstPlatformAvailability() const
{
    PlatformAvailability best;
    best.freeAt = currentTime + std::chrono::hours(24 * 365);

    for (const auto& platform : platforms)
    {
        const int id = platform->getPlatformId();
        TimePoint platformFreeAt = currentTime;
        for (const auto& a : assignments)
        {
            if ((a.status == TrainAssignmentStatus::ASSIGNED ||
                 a.status == TrainAssignmentStatus::DELAYED ||
                 a.status == TrainAssignmentStatus::ARRIVED) &&
                a.platformId == id)
            {
                const TimePoint endTime = a.predictedArrival + a.stopTime;
                if (endTime > platformFreeAt) { platformFreeAt = endTime; }
            }
        }
        if (platformFreeAt < best.freeAt)
        {
            best.freeAt = platformFreeAt;
            best.platformId = id;
        }
    }
    return best;
}

void Station::UpdateTime(TimePoint now)
{ currentTime = now; }

void Station::AddPassengerTrain(const TimePoint& timeDest, const TimePoint& timeDep)
{
    trains.push_back(std::make_unique<PassengerTrain>(TrainId++, timeDest, timeDep));
    SortTrains(trains);
}

void Station::AddPassengerTrain(const TimePoint& timeDest, const RealDuration& stopTime)
{
    const TimePoint timeDep = timeDest + stopTime;
    trains.push_back(std::make_unique<PassengerTrain>(TrainId++, timeDest, timeDep));
    SortTrains(trains);
}

void Station::AddPlatform(const std::string& number)
{
    platforms.push_back(std::make_unique<Platform>(platformId++, number));
}

void Station::ShowTrains() const
{
    if (trains.empty())
    {
        printf("No trains in station.\n");
        return;
    }
    for (const auto& train : trains)
    {
        std::cout << train->ToString() << std::endl;
    }
}
