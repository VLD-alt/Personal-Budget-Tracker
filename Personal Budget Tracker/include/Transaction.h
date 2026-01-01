#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

class Transaction {
protected:
    int id_;
    double amount_;
    string category_;
    string date_;   
    string note_;

public:
    Transaction(int id, double amount, string category, string date, string note);
    virtual ~Transaction() = default;

    int id() const;
    double amount() const;
    string category() const;
    string date() const;
    string note() const;

    void setAmount(double a);
    void setCategory(const string& c);
    void setDate(const string& d);
    void setNote(const string& n);

    virtual string type() const = 0;          
    virtual double signedAmount() const = 0;  

    virtual string toLine() const;
    virtual void printRow() const;
};

#endif

