#pragma once
#include <string>

class Train;

class Platform
{
private:
    int id;
    std::string number;
    bool isFree = true;

public:
    Platform(int id, std::string number);

    bool getStatusPlatform() const { return isFree; }
    int getPlatformId() const { return id; }
    const std::string& getNumber() const { return number; }
    void setStatusPlatform(bool free) { isFree = free; }

};
