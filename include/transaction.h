#pragma once
#include <fstream>
#include <set>
#include <map>

struct Transaction {
    char id{};
    char result{};
    std::set<char> dependencies;

    // check if at least one of two transactions depends on one another.
    static bool areDependent(const Transaction& first, const Transaction& second);

    // get all transactions from the file mapped by their ids
    static std::map<char, Transaction> getTransactions(const std::string& path, const std::set<char>& alphabet);

    // print the transaction
    void print() const;

private:
    // parse a transaction from a line in the file
    static Transaction readTransaction(const std::string& line);

    // simple io function to read a file to the "data" string
    static void readFile(std::string& data, const std::string& path);
};
