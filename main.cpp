#include <iostream>
#include <map>

#include "include/transaction.h"

const std::string INPUT_FILE = "../transactions.txt";

int main() {
    std::map<char, Transaction> transactions;
    getTransactions(transactions, INPUT_FILE);
    for (auto const& [key, val] : transactions) {
        std::cout << key << std::endl;
        val.print();
        std::cout << std::endl;
    }
}
