#ifndef EXPENSE_H
#define EXPENSE_H

#include "Transaction.h"

using namespace std;

class Expense : public Transaction {
public:
    using Transaction::Transaction;

    string type() const override { return "E"; }
    double signedAmount() const override { return -amount_; }
};

#endif
