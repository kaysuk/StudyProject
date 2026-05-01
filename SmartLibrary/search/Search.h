#pragma once
#include <memory>
#include <vector>
#include <string>

class Book;

class Search
{
private:
    int calculateScore(const Book* book, const std::string& query) const;

    std::string toLower(const std::string& str) const;

public:
    std::vector<const Book*> search(
        const std::vector<std::unique_ptr<Book>>& books,
        const std::string& query
    ) const;
};
