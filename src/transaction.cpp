#include "../include/transaction.h"
#include <iostream>
#include <sstream>
#include <cctype>

void Transaction::print() const {
    std::cout << "id: " << id << std::endl;
    std::cout << "result: " << result << std::endl;
    std::cout << "dependencies: ";
    for (const auto dependency : dependencies) {
        std::cout << dependency << " ";
    }
    std::cout << std::endl;
}

void readFile(std::string& data, const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while(getline(file, line)) {
        data += line + "\n";
    }
    file.close();
}

Transaction readTransaction(const std::string& line) {
    Transaction transaction;
    if(line.length() < 9) {
        throw std::invalid_argument("Transaction too short: " + line);
    }
    if(line[0] != '(' || line.substr(2, 2) != ") "
        || line.substr(5, 3) != " :=") {
        throw std::invalid_argument("Invalid transaction format: " + line);
        }
    if(!isalpha(line[4])) {
        throw std::invalid_argument("Transaction result should be a letter: " + line);
    }
    transaction.id = line[1];
    transaction.result = line[4];
    for(int i = 8; i < line.length(); i++) {
        if(isalpha(line[i])) {
            transaction.dependencies.insert(line[i]);
        }
    }
    return transaction;
}

void getTransactions(std::map<char, Transaction>& transactions, const std::string& file) {
    std::string transactionsRaw;
    readFile(transactionsRaw, file);
    std::stringstream stream(transactionsRaw);
    for(std::string line; std::getline(stream, line);) {
        Transaction transaction = readTransaction(line);
        if(transactions.contains(transaction.id)) {
            throw std::invalid_argument("Transaction already exists: " + transaction.id);
        }
        transactions[transaction.id] = transaction;
    }
}
