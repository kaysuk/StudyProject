#pragma once
#include <string>

class Train
{
protected:
    int id;
    std::string number;
    std::string departure;
    std::string destination;
    std::string status;

public:
    virtual void displayInfo() const = 0;
    virtual ~Train() {}

};
