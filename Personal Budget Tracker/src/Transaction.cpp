#include "Transaction.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <utility>

using namespace std;

Transaction::Transaction(int id, double amount, string category, string date, string note)
    : id_(id), amount_(amount), category_(move(category)), date_(move(date)), note_(move(note)) {
}

int Transaction::id() const { return id_; }
double Transaction::amount() const { return amount_; }
string Transaction::category() const { return category_; }
string Transaction::date() const { return date_; }
string Transaction::note() const { return note_; }

void Transaction::setAmount(double a) { amount_ = a; }
void Transaction::setCategory(const string& c) { category_ = c; }
void Transaction::setDate(const string& d) { date_ = d; }
void Transaction::setNote(const string& n) { note_ = n; }

string Transaction::toLine() const {
    ostringstream oss;
    oss << id_ << "|" << type() << "|"
        << fixed << setprecision(2) << amount_ << "|"
        << category_ << "|" << date_ << "|" << note_;
    return oss.str();
}

void Transaction::printRow() const {
    cout << left << setw(5) << id_
        << setw(8) << (type() == "I" ? "Income" : "Expense")
        << setw(12) << fixed << setprecision(2) << amount_
        << setw(15) << category_
        << setw(12) << date_
        << note_ << "\n";
}
