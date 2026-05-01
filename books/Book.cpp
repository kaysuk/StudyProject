#include "Book.h"
#include <iostream>

Book::Book(int bookId, std::string t, std::string a): id(bookId), title(t), author(a) {}

void Book::Info() const
{
    std::cout << ToString() << std::endl;
}
