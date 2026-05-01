#pragma once
#include <string>
#include <vector>

class Book;
class Library;

class User
{
    int id;
    std::string name;
    std::string email;
    std::string password;

public:
    User(int id, std::string name, std::string email, std::string password);
    void UserInfo() const;
    void UserName() const;

    int GetId() const { return id; }
    const std::string& GetName() const { return name; }
    const std::string& GetEmail() const { return email; }
    const std::string& GetPassword() const { return password; }
};
