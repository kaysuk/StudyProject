#include "Platform.h"

Platform::Platform(int id, std::string number) :
    id(id), number(std::move(number)) {}
