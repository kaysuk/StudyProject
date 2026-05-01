#include "User.h"
#include <cstdio>

User::User(int userId, std::string user_name, std::string user_email, std::string user_password)
    : id(userId), name(user_name), email(user_email), password(user_password) {}

void User::UserInfo() const
{
    printf("[%d] name: %s\nemail: %s\n", id, name.c_str(), email.c_str());
}

void User::UserName() const
{
    printf("[%d] name: %s\n", id, name.c_str());
}
