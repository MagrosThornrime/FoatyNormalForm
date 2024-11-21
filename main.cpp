#include <iostream>
#include <map>
#include <vector>

#include "include/transaction.h"

const std::string INPUT_FILE = "../transactions.txt";

struct Node {
    char transactionID;
    std::vector<int> neighbors;
};

struct Tree {
    std::vector<Node> nodes;
};

struct Edge {
    char from;
    char to;
};

void dependencyGraphs(std::vector<Edge>& dependentEdges,
                    std::vector<Edge>& independentEdges,
                    const std::map<char, Transaction>& transactions) {
    std::vector<char> keys;
    for (auto const& [key, _] : transactions) {
        keys.push_back(key);
    }

    for(int i = 0; i < keys.size(); i++) {
        for(int j=0; j<keys.size(); j++) {
            const Transaction& first = transactions.at(keys[i]);
            const Transaction& second = transactions.at(keys[j]);
            if(Transaction::areDependent(first, second)) {
                dependentEdges.push_back(Edge{first.id, second.id});
            }
            else {
                independentEdges.push_back(Edge{first.id, second.id});
            }
        }
    }

}

int main() {
    std::map<char, Transaction> transactions;
    Transaction::getTransactions(transactions, INPUT_FILE);
    // for (auto const& [key, val] : transactions) {
    //     std::cout << key << std::endl;
    //     val.print();
    //     std::cout << std::endl;
    // }
    std::vector<Edge> dependentEdges, independentEdges;
    dependencyGraphs(dependentEdges, independentEdges, transactions);
    std::cout << "{";
    for(const auto&[from, to] : dependentEdges) {
        std::cout << "(" << from << "," << to << "), ";
    }
    std::cout << "}" << std::endl;
    std::cout << "{";
    for(const auto&[from, to] : independentEdges) {
        std::cout << "(" << from << "," << to << "), ";
    }
    std::cout << "}" << std::endl;
}
