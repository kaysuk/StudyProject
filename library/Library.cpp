#include "Library.h"
#include "books/Book.h"
#include "books/PrintedBook.h"
#include "users/User.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <sstream>

namespace
{
std::string Normalize(const std::string& value)
{
    std::string normalized = value;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return normalized;
}
}

Library::~Library() = default;

Book* Library::FindBookByIdInternal(int bookId)
{
    for (auto& book : books)
    {
        if (book->getId() == bookId)
        {
            return book.get();
        }
    }
    return nullptr;
}

const Book* Library::FindBookByIdInternal(int bookId) const
{
    for (const auto& book : books)
    {
        if (book->getId() == bookId)
        {
            return book.get();
        }
    }
    return nullptr;
}

User* Library::FindUserByIdInternal(int userId)
{
    for (auto& user : users)
    {
        if (user->GetId() == userId)
        {
            return user.get();
        }
    }
    return nullptr;
}

const User* Library::FindUserByIdInternal(int userId) const
{
    for (const auto& user : users)
    {
        if (user->GetId() == userId)
        {
            return user.get();
        }
    }
    return nullptr;
}

LoanRecord* Library::FindLoanByBookIdInternal(int bookId)
{
    for (auto& loan : loans)
    {
        if (loan.bookId == bookId)
        {
            return &loan;
        }
    }
    return nullptr;
}

const LoanRecord* Library::FindLoanByBookIdInternal(int bookId) const
{
    for (const auto& loan : loans)
    {
        if (loan.bookId == bookId)
        {
            return &loan;
        }
    }
    return nullptr;
}

bool Library::HasDuplicateBook(const std::string& title, const std::string& author) const
{
    const std::string normalizedTitle = Normalize(title);
    const std::string normalizedAuthor = Normalize(author);

    for (const auto& book : books)
    {
        if (Normalize(book->getTitle()) == normalizedTitle &&
            Normalize(book->getAuthor()) == normalizedAuthor)
        {
            return true;
        }
    }
    return false;
}

bool Library::HasDuplicateUserEmail(const std::string& email) const
{
    const std::string normalizedEmail = Normalize(email);
    for (const auto& user : users)
    {
        if (Normalize(user->GetEmail()) == normalizedEmail)
        {
            return true;
        }
    }
    return false;
}

OperationResult Library::AddUser(const std::string& name, const std::string& email, const std::string& password)
{
    if (name.empty() || email.empty() || password.empty())
    {
        return {false, "Name, email and password must not be empty."};
    }

    if (HasDuplicateUserEmail(email))
    {
        return {false, "User with this email already exists."};
    }

    users.push_back(std::make_unique<User>(nextUserId++, name, email, password));
    return {true, "User added successfully."};
}

OperationResult Library::AddPrintedBook(const std::string& title, const std::string& author, unsigned int pages)
{
    if (title.empty() || author.empty())
    {
        return {false, "Title and author must not be empty."};
    }

    if (pages == 0)
    {
        return {false, "Pages must be greater than zero."};
    }

    if (HasDuplicateBook(title, author))
    {
        return {false, "Book with the same title and author already exists."};
    }

    books.push_back(std::make_unique<PrintedBook>(nextBookId++, title, author, pages));
    return {true, "Book added successfully."};
}

OperationResult Library::GiveBook(int userId, int bookId, int loanDays)
{
    if (loanDays <= 0)
    {
        return {false, "Loan period must be greater than zero days."};
    }

    const User* user = FindUserByIdInternal(userId);
    if (!user)
    {
        return {false, "User not found."};
    }

    const Book* book = FindBookByIdInternal(bookId);
    if (!book)
    {
        return {false, "Book not found."};
    }

    if (FindLoanByBookIdInternal(bookId))
    {
        return {false, "Book is already borrowed."};
    }

    const std::time_t now = std::time(nullptr);
    const std::time_t due = now + static_cast<std::time_t>(loanDays) * 24 * 60 * 60;
    loans.push_back({bookId, userId, now, due});

    std::ostringstream message;
    message << "Book issued to " << user->GetName()
            << ". Due date: " << FormatTime(due);
    return {true, message.str()};
}

OperationResult Library::ReturnBook(int userId, int bookId)
{
    const User* user = FindUserByIdInternal(userId);
    if (!user)
    {
        return {false, "User not found."};
    }

    const Book* book = FindBookByIdInternal(bookId);
    if (!book)
    {
        return {false, "Book not found."};
    }

    for (auto it = loans.begin(); it != loans.end(); ++it)
    {
        if (it->bookId == bookId && it->userId == userId)
        {
            loans.erase(it);
            return {true, "Book returned successfully."};
        }
    }

    if (FindLoanByBookIdInternal(bookId))
    {
        return {false, "This book is borrowed by another user."};
    }

    return {false, "This book is not currently borrowed."};
}

void Library::showBooks() const
{
    if (books.empty())
    {
        printf("No books in library.\n");
        return;
    }

    for (const auto& book : books)
    {
        if (!IsBookBorrowed(book->getId()))
        {
            book->Info();
        }
    }
}

void Library::showUsers() const
{
    if (users.empty())
    {
        printf("No users registered.\n");
        return;
    }

    for (const auto& user : users)
    {
        user->UserInfo();
    }
}

void Library::showBorrowedBooks() const
{
    if (loans.empty())
    {
        printf("No active loans.\n");
        return;
    }

    for (const auto& loan : loans)
    {
        const User* user = FindUserByIdInternal(loan.userId);
        const Book* book = FindBookByIdInternal(loan.bookId);
        if (!user || !book)
        {
            continue;
        }

        printf("User: %s [%d]\n", user->GetName().c_str(), user->GetId());
        printf("Book: %s\n", book->ToString().c_str());
        printf("Borrowed: %s\n", FormatTime(loan.borrowedAt).c_str());
        printf("Due: %s\n\n", FormatTime(loan.dueAt).c_str());
    }
}

const std::vector<std::unique_ptr<Book>>& Library::get_vectorBooks() const
{
    return books;
}

const std::vector<std::unique_ptr<User>>& Library::getUsers() const
{
    return users;
}

const std::vector<LoanRecord>& Library::GetLoans() const
{
    return loans;
}

const Book* Library::FindBookById(int bookId) const
{
    return FindBookByIdInternal(bookId);
}

const User* Library::FindUserById(int userId) const
{
    return FindUserByIdInternal(userId);
}

bool Library::IsBookBorrowed(int bookId) const
{
    return FindLoanByBookIdInternal(bookId) != nullptr;
}

std::optional<LoanRecord> Library::GetLoanByBookId(int bookId) const
{
    const LoanRecord* loan = FindLoanByBookIdInternal(bookId);
    if (!loan)
    {
        return std::nullopt;
    }
    return *loan;
}

std::string Library::FormatTime(std::time_t timeValue)
{
    std::tm tmValue = *std::localtime(&timeValue);
    std::ostringstream out;
    out << std::put_time(&tmValue, "%Y-%m-%d %H:%M");
    return out.str();
}
