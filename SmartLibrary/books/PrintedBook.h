#pragma once
#include "Book.h"

class PrintedBook : public Book
{
    unsigned int pages;
public:
    PrintedBook(int id, std::string t, std::string a, unsigned int p);
    std::string ToString() const override;
};
