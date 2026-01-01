#include "FileStorage.h"
#include "Transaction.h"
#include "Income.h"
#include "Expense.h"
#include <filesystem>
#include <iostream>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>

using namespace std;

static inline string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    if (b == string::npos) return "";
    return s.substr(b, e - b + 1);
}

bool FileStorage::save(const string& filename,
    const vector<unique_ptr<Transaction>>& data) {
    
    ofstream out(filename);
    if (!out) return false;

    for (const auto& t : data) out << t->toLine() << "\n";
    return true;
}

bool FileStorage::load(const string& filename,
    vector<unique_ptr<Transaction>>& data,
    int& nextId) {
    ifstream in(filename);
  
    if (!in) return false; 

    data.clear();
    string line;
    int maxId = 0;

    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        //
        vector<string> parts;
        string token;
        stringstream ss(line);

        while (getline(ss, token, '|')) parts.push_back(token);

       
        if (!line.empty() && line.back() == '|') parts.push_back("");

        
        if (parts.size() < 6) continue;
        //

        int id = stoi(parts[0]);
        string t = parts[1];
        double amount = stod(parts[2]);
        string category = parts[3];
        string date = parts[4];
        string note = parts[5];

        unique_ptr<Transaction> ptr;
        if (t == "I") ptr = make_unique<Income>(id, amount, category, date, note);
        else if (t == "E") ptr = make_unique<Expense>(id, amount, category, date, note);
        else continue;

        maxId = max(maxId, id);
        data.push_back(move(ptr));
    }

    nextId = maxId + 1;
    return true;
}
