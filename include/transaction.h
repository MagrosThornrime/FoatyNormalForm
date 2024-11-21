#pragma once
#include <fstream>
#include <set>
#include <map>

struct Transaction {
    char id{};
    char result{};
    std::set<char> dependencies;

    void print() const;
};

void readFile(std::string& data, const std::string& path);

Transaction readTransaction(const std::string& line);

void getTransactions(std::map<char, Transaction>& transactions, const std::string& file);
