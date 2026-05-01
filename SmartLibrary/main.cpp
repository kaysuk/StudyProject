#include "books/Book.h"
#include "library/Library.h"
#include "search/Search.h"
#include <iostream>
#include <limits>
#include <string>

namespace
{
void PrintMenu()
{
    std::cout << "\nSmartLibrary Menu\n"
              << "1. Show available books\n"
              << "2. Show users\n"
              << "3. Show active loans\n"
              << "4. Add printed book\n"
              << "5. Add user\n"
              << "6. Borrow book\n"
              << "7. Return book\n"
              << "8. Search books\n"
              << "0. Exit\n"
              << "Choose: ";
}

void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int ReadInt(const std::string& prompt)
{
    int value = 0;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            ClearInput();
            return value;
        }

        std::cout << "Invalid number. Try again.\n";
        ClearInput();
    }
}

std::string ReadLine(const std::string& prompt)
{
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

void PrintResult(const OperationResult& result)
{
    std::cout << (result.success ? "[OK] " : "[ERROR] ") << result.message << "\n";
}

void SeedData(Library& library)
{
    library.AddUser("Dmitriy", "dmitriy@mail.ru", "123");
    library.AddUser("Anna", "anna@mail.ru", "456");
    library.AddPrintedBook("1984", "George Orwell", 328);
    library.AddPrintedBook("The Hobbit", "J.R.R. Tolkien", 310);
    library.AddPrintedBook("Dune", "Frank Herbert", 412);
    library.AddPrintedBook("War and Peace", "Leo Tolstoy", 1225);
}
}

int main()
{
    Library library;
    Search search;
    SeedData(library);

    while (true)
    {
        PrintMenu();

        int choice = -1;
        if (!(std::cin >> choice))
        {
            std::cout << "Invalid menu choice.\n";
            ClearInput();
            continue;
        }
        ClearInput();

        if (choice == 0)
        {
            break;
        }

        switch (choice)
        {
            case 1:
                library.showBooks();
                break;
            case 2:
                library.showUsers();
                break;
            case 3:
                library.showBorrowedBooks();
                break;
            case 4:
            {
                const std::string title = ReadLine("Title: ");
                const std::string author = ReadLine("Author: ");
                const int pages = ReadInt("Pages: ");
                PrintResult(library.AddPrintedBook(title, author, static_cast<unsigned int>(pages)));
                break;
            }
            case 5:
            {
                const std::string name = ReadLine("Name: ");
                const std::string email = ReadLine("Email: ");
                const std::string password = ReadLine("Password: ");
                PrintResult(library.AddUser(name, email, password));
                break;
            }
            case 6:
            {
                const int userId = ReadInt("User ID: ");
                const int bookId = ReadInt("Book ID: ");
                const int loanDays = ReadInt("Loan days: ");
                PrintResult(library.GiveBook(userId, bookId, loanDays));
                break;
            }
            case 7:
            {
                const int userId = ReadInt("User ID: ");
                const int bookId = ReadInt("Book ID: ");
                PrintResult(library.ReturnBook(userId, bookId));
                break;
            }
            case 8:
            {
                const std::string query = ReadLine("Search query: ");
                const auto results = search.search(library.get_vectorBooks(), query);
                if (results.empty())
                {
                    std::cout << "No matches found.\n";
                    break;
                }

                for (const Book* book : results)
                {
                    book->Info();
                    if (const auto loan = library.GetLoanByBookId(book->getId()))
                    {
                        std::cout << "  Borrowed until: " << Library::FormatTime(loan->dueAt) << "\n";
                    }
                }
                break;
            }
            default:
                std::cout << "Unknown menu choice.\n";
                break;
        }
    }

    return 0;
}
