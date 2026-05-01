#include "PrintedBook.h"
#include <sstream>

PrintedBook::PrintedBook(int id, std::string t, std::string a, unsigned int p): Book(id, t, a), pages(p) {}

std::string PrintedBook::ToString() const
{
    std::ostringstream out;
    out << "[" << id << "] " << title << " by " << author
        << ", pages: " << pages;
    return out.str();
}
