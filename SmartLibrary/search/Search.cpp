#include "Search.h"
#include "books/Book.h"
#include <algorithm>
#include <cctype>
#include <iostream>

struct ScoredBook
{
    const Book* book;
    int score;
};

std::string Search::toLower(const std::string& str) const
{
    std::string result = str;

    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {return std::tolower(c); });

    return result;
}

int Search::calculateScore(const Book* book, const std::string& query) const
{
    int score = 0;

    std::string title = toLower(book->getTitle());
    std::string author = toLower(book->getAuthor());
    std::string q = toLower(query);

    if (title == q) score += 100;
    if (author == q) score += 90;
    if (title.find(q) == 0) score += 50;
    if (author.find(q) == 0) score += 40;
    if (title.find(q) != std::string::npos) score += 20;
    if (author.find(q) != std::string::npos) score += 10;

    return score;
}

std::vector<const Book*> Search::search(
    const std::vector<std::unique_ptr<Book>>& books,
    const std::string& query
) const
{
    std::vector<ScoredBook> scored;

    for (const auto& book : books)
    {
        int score = calculateScore(book.get(), query);
        if (score > 0)
        {
            scored.push_back({book.get(), score});
        }
    }

    std::sort(scored.begin(), scored.end(), [](const ScoredBook& lhs, const ScoredBook& rhs) {
        return lhs.score > rhs.score;
    });

    std::vector<const Book*> result;
    for (const auto& scoredBook : scored)
    {
        result.push_back(scoredBook.book);
    }
    return result;
}
