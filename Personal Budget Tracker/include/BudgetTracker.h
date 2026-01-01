#ifndef BUDGETTRACKER_H
#define BUDGETTRACKER_H

#include <string>
#include <vector>
#include <memory>

#include "Transaction.h"   

using namespace std;

class BudgetTracker {
private:
    vector<unique_ptr<Transaction>> transactions_;
    int nextId_ = 1;
    string filename_;

    Transaction* findById(int id);

public:
    explicit BudgetTracker(string filename);

    void save();

    void addIncome(double amount, const string& category,
        const string& date, const string& note);

    void addExpense(double amount, const string& category,
        const string& date, const string& note);

    bool editTransaction(int id, double amount, const string& category,
        const string& date, const string& note);

    bool deleteTransaction(int id);

    void deleteAllTransactions();

    void listAll() const;
    void filterByCategory(const string& cat) const;
    void filterByDate(const string& date) const;

    double totalIncome() const;
    double totalExpense() const;
    double balance() const;

    void report() const;
};

#endif
