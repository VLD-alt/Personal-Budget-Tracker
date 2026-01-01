#ifndef INCOME_H
#define INCOME_H

#include "Transaction.h"

using namespace std;

class Income : public Transaction {
public:
    using Transaction::Transaction;

    string type() const override { return "I"; }
    double signedAmount() const override { return amount_; }
};

#endif

