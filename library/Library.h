#pragma once
#include <ctime>
#include "books/Book.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "users/User.h"

struct OperationResult
{
    bool success;
    std::string message;
};

struct LoanRecord
{
    int bookId;
    int userId;
    std::time_t borrowedAt;
    std::time_t dueAt;
};

class Library
{
    std::vector<std::unique_ptr<Book>> books;
    std::vector<std::unique_ptr<User>> users;
    std::vector<LoanRecord> loans;
    int nextBookId = 1;
    int nextUserId = 1;

    Book* FindBookByIdInternal(int bookId);
    const Book* FindBookByIdInternal(int bookId) const;
    User* FindUserByIdInternal(int userId);
    const User* FindUserByIdInternal(int userId) const;
    LoanRecord* FindLoanByBookIdInternal(int bookId);
    const LoanRecord* FindLoanByBookIdInternal(int bookId) const;
    bool HasDuplicateBook(const std::string& title, const std::string& author) const;
    bool HasDuplicateUserEmail(const std::string& email) const;

public:
    ~Library();

    OperationResult AddUser(const std::string& name, const std::string& email, const std::string& password);
    OperationResult AddPrintedBook(const std::string& title, const std::string& author, unsigned int pages);

    OperationResult GiveBook(int userId, int bookId, int loanDays);
    OperationResult ReturnBook(int userId, int bookId);

    void showBooks() const;
    void showUsers() const;
    void showBorrowedBooks() const;

    const std::vector<std::unique_ptr<Book>>& get_vectorBooks() const;
    const std::vector<std::unique_ptr<User>>& getUsers() const;
    const std::vector<LoanRecord>& GetLoans() const;
    const Book* FindBookById(int bookId) const;
    const User* FindUserById(int userId) const;
    bool IsBookBorrowed(int bookId) const;
    std::optional<LoanRecord> GetLoanByBookId(int bookId) const;
    static std::string FormatTime(std::time_t timeValue);
};
