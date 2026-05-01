#pragma once
#include <string>

class Book
{
protected:
    int id;
    std::string title;
    std::string author;
public:
    Book(int id, std::string t, std::string a);

    virtual std::string ToString() const = 0;
    virtual void Info() const;

    virtual ~Book() = default;

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
};
