#include "BudgetTracker.h"
#include "Income.h"
#include "Expense.h"
#include "FileStorage.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

using namespace std;

BudgetTracker::BudgetTracker(string filename) : filename_(move(filename)) {
    FileStorage::load(filename_, transactions_, nextId_);
}

void BudgetTracker::save() {
    if (!FileStorage::save(filename_, transactions_)) {
        cout << "Warning: could not save file.\n";
    }
}

Transaction* BudgetTracker::findById(int id) {
    for (auto& t : transactions_) {
        if (t->id() == id) return t.get();
    }
    return nullptr;
}

void BudgetTracker::addIncome(double amount, const string& category,
    const string& date, const string& note) {
    transactions_.push_back(make_unique<Income>(nextId_++, amount, category, date, note));
    save();
}

void BudgetTracker::addExpense(double amount, const string& category,
    const string& date, const string& note) {
    transactions_.push_back(make_unique<Expense>(nextId_++, amount, category, date, note));
    save();
}

bool BudgetTracker::editTransaction(int id, double amount, const string& category,
    const string& date, const string& note) {
    Transaction* t = findById(id);
    if (!t) return false;

    t->setAmount(amount);
    t->setCategory(category);
    t->setDate(date);
    t->setNote(note);
    save();
    return true;
}

bool BudgetTracker::deleteTransaction(int id) {
    auto it = remove_if(transactions_.begin(), transactions_.end(),
        [&](const unique_ptr<Transaction>& t) { return t->id() == id; });

    if (it == transactions_.end()) return false;
    transactions_.erase(it, transactions_.end());
    save();
    return true;
}

void BudgetTracker::deleteAllTransactions() {
    transactions_.clear();
    nextId_ = 1;
    save();
}

void BudgetTracker::listAll() const {
    if (transactions_.empty()) {
        cout << "No transactions yet.\n";
        return;
    }

    cout << left << setw(5) << "ID"
        << setw(8) << "Type"
        << setw(12) << "Amount"
        << setw(15) << "Category"
        << setw(12) << "Date"
        << "Note\n";
    cout << string(70, '-') << "\n";

    for (const auto& t : transactions_) t->printRow();
}

void BudgetTracker::filterByCategory(const string& cat) const {
    bool any = false;
    for (const auto& t : transactions_) {
        if (t->category() == cat) {
            if (!any) {
                cout << "Transactions in category: " << cat << "\n";
                cout << string(70, '-') << "\n";
            }
            t->printRow();
            any = true;
        }
    }
    if (!any) cout << "No transactions found for this category.\n";
}

void BudgetTracker::filterByDate(const string& date) const {
    bool any = false;
    for (const auto& t : transactions_) {
        if (t->date() == date) {
            if (!any) {
                cout << "Transactions on date: " << date << "\n";
                cout << string(70, '-') << "\n";
            }
            t->printRow();
            any = true;
        }
    }
    if (!any) cout << "No transactions found for this date.\n";
}

double BudgetTracker::totalIncome() const {
    double s = 0;
    for (const auto& t : transactions_) {
        if (t->type() == "I") s += t->amount();
    }
    return s;
}

double BudgetTracker::totalExpense() const {
    double s = 0;
    for (const auto& t : transactions_) {
        if (t->type() == "E") s += t->amount();
    }
    return s;
}

double BudgetTracker::balance() const {
    double s = 0;
    for (const auto& t : transactions_) s += t->signedAmount();
    return s;
}

void BudgetTracker::report() const {
    double inc = totalIncome();
    double exp = totalExpense();
    double bal = balance();

    cout << "\n=== Financial Report ===\n";
    cout << "Total income:   " << fixed << setprecision(2) << inc << "\n";
    cout << "Total expenses: " << fixed << setprecision(2) << exp << "\n";
    cout << "Balance:        " << fixed << setprecision(2) << bal << "\n\n";

    vector<string> cats;
    for (const auto& t : transactions_) cats.push_back(t->category());

    sort(cats.begin(), cats.end());
    cats.erase(unique(cats.begin(), cats.end()), cats.end());

    cout << "Category summary (expenses):\n";
    bool printed = false;

    for (const auto& c : cats) {
        double sum = 0;
        for (const auto& t : transactions_) {
            if (t->type() == "E" && t->category() == c) sum += t->amount();
        }
        if (sum > 0.00001) {
            cout << " - " << c << ": " << fixed << setprecision(2) << sum << "\n";
            printed = true;
        }
    }

    if (!printed) cout << " (no expenses yet)\n";
    cout << "\n";
}
