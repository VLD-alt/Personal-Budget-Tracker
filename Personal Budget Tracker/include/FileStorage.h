#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

class Transaction;

class FileStorage {
public:
    static bool save(const string& filename,
        const vector<unique_ptr<Transaction>>& data);

    static bool load(const string& filename,
        vector<unique_ptr<Transaction>>& data,
        int& nextId);
};

#endif

