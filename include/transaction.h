#pragma once
#include <fstream>
#include <set>
#include <map>

struct Transaction {
    char id{};
    char result{};
    std::set<char> dependencies;

    static bool areDependent(const Transaction& first, const Transaction& second);
    static void getTransactions(std::map<char, Transaction>& transactions, const std::string& path);
    void print() const;

private:
    static Transaction readTransaction(const std::string& line);
    static void readFile(std::string& data, const std::string& path);
};
