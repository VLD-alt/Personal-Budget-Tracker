#include <iostream>
#include <string>
#include <cctype>
#include <filesystem>
#include <utility> 
#include "BudgetTracker.h"

using namespace std;

static inline string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    if (b == string::npos) return "";
    return s.substr(b, e - b + 1);
}

static bool isValidDate(const string& d) {
    if (d.size() != 10) return false;
    if (d[4] != '-' || d[7] != '-') return false;

    for (int i : {0, 1, 2, 3, 5, 6, 8, 9}) {
        if (!isdigit(static_cast<unsigned char>(d[i]))) return false;
    }

    int y = stoi(d.substr(0, 4));
    int m = stoi(d.substr(5, 2));
    int day = stoi(d.substr(8, 2));

    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (day < 1 || day > 31) return false;
    return true;
}

static double readDouble(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);

        try {
            double v = stod(s);
            if (v < 0) {
                cout << "Amount must be >= 0.\n";
                continue;
            }
            return v;
        }
        catch (...) {
            cout << "Invalid number, try again.\n";
        }
    }
}

static int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);

        try {
            return stoi(s);
        }
        catch (...) {
            cout << "Invalid integer, try again.\n";
        }
    }
}

static string readDate(const string& prompt) {
    while (true) {
        cout << prompt;
        string d;
        getline(cin, d);
        d = trim(d);

        if (isValidDate(d)) return d;
        cout << "Invalid date. Use YYYY-MM-DD.\n";
    }
}

static string readNonEmpty(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);

        if (!s.empty()) return s;
        cout << "Cannot be empty.\n";
    }
}

static bool readYesNo(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (s.empty()) continue;

        char c = static_cast<char>(tolower(static_cast<unsigned char>(s[0])));
        if (c == 'y') return true;
        if (c == 'n') return false;

        cout << "Please type y or n.\n";
    }
}

int main() {
    

    BudgetTracker app("budget_data.txt");

    while (true) {
        cout << "==============================\n";
        cout << " Personal Budget Tracker (C++)\n";
        cout << "==============================\n";
        cout << "1) Add income\n";
        cout << "2) Add expense\n";
        cout << "3) Edit transaction\n";
        cout << "4) Delete transaction\n";
        cout << "5) View all transactions\n";
        cout << "6) Filter by category\n";
        cout << "7) Filter by date range\n";
        cout << "8) Show report\n";
        cout << "0) Exit\n";

        int choice = readInt("Choose option: ");
        cout << "\n";

        if (choice == 0) {
            cout << "Bye!\n";
            break;
        }
        else if (choice == 1) {
            double amount = readDouble("Amount: ");
            string category = readNonEmpty("Category: ");
            string date = readDate("Date (YYYY-MM-DD): ");
            cout << "Note (optional): ";
            string note; getline(cin, note);

            app.addIncome(amount, category, date, trim(note));
            cout << "Income added.\n\n";
        }
        else if (choice == 2) {
            double amount = readDouble("Amount: ");
            string category = readNonEmpty("Category: ");
            string date = readDate("Date (YYYY-MM-DD): ");
            cout << "Note (optional): ";
            string note; getline(cin, note);

            app.addExpense(amount, category, date, trim(note));
            cout << "Expense added.\n\n";
        }
        else if (choice == 3) {
            int id = readInt("Transaction ID to edit: ");
            double amount = readDouble("New amount: ");
            string category = readNonEmpty("New category: ");
            string date = readDate("New date (YYYY-MM-DD): ");
            cout << "New note (optional): ";
            string note; getline(cin, note);

            if (app.editTransaction(id, amount, category, date, trim(note)))
                cout << "Transaction updated.\n\n";
            else
                cout << "Transaction not found.\n\n";
        }
        else if (choice == 4) {
            cout << "Current transactions:\n";
            app.listAll();
            cout << "\n";

            cout << "Enter transaction ID to delete or type all to delete everything: ";
            string input;
            getline(cin, input);
            input = trim(input);

            if (input.empty()) {
                cout << "Cancelled.\n\n";
                continue;
            }

            string low = input;
            for (char& ch : low) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));

            if (low == "all") {
                cout << "WARNING: this will delete ALL transactions.\n";
                if (!readYesNo("Are you sure? (y/n): ")) {
                    cout << "Cancelled.\n\n";
                    continue;
                }

                app.deleteAllTransactions();
                cout << "All transactions deleted.\n\n";
            }
            else {
                int id;
                try {
                    id = stoi(input);
                }
                catch (...) {
                    cout << "Invalid input. Enter a number ID or ALL.\n\n";
                    continue;
                }

                if (!readYesNo("Are you sure? (y/n): ")) {
                    cout << "Cancelled.\n\n";
                    continue;
                }

                if (app.deleteTransaction(id))
                    cout << "Transaction deleted.\n\n";
                else
                    cout << "Transaction not found.\n\n";
            }
        }
        else if (choice == 5) {
            app.listAll();
            cout << "\n";
        }
        else if (choice == 6) {
            string cat = readNonEmpty("Category to filter: ");
            app.filterByCategory(cat);
            cout << "\n";
        }
        else if (choice == 7) {
            string fromDate = readDate("From date(YYYY-MM-DD): ");
            string toDate = readDate("To date (YYYY-MM-DD): ");
            if (fromDate > toDate) {

                swap(fromDate, toDate); 
                cout << "Swapped dates to keep range .\n";


            }
            app.filterByDateRange(fromDate, toDate);
            cout << "\n";




        }
        else if (choice == 8) {
            app.report();
        }
        else {
            cout << "Unknown option.\n\n";
        }
    }

    return 0;
}

