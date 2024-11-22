#include <iostream>
#include <map>
#include <vector>
#include <ranges>

#include "include/transaction.h"

const std::string INPUT_FILE = "../transactions.txt";
const std::string word = "baadcb";
const std::set alphabet = {'a', 'b', 'c', 'd'};

struct Node {
    char transactionID;
    std::vector<int> neighbors;

};

struct Graph {
    std::vector<Node> nodes;

    void printEdges() {
        std::cout << "{";
        for (const auto& node : nodes) {
            for(const auto& neighbor : node.neighbors) {
                char neighborID = nodes.at(neighbor).transactionID;
                std::cout << "(" << node.transactionID << "," << neighborID << "), ";
            }
        }
        std::cout << "}" << std::endl;
    }
};

struct DependencyGraph : Graph {
    std::map<char, int> indexes;

    bool containsNode(char transactionID) const {
        return indexes.contains(transactionID);
    }

    bool containsEdge(char key1, char key2) const {
        if(!indexes.contains(key1) || !indexes.contains(key2)) {
            return false;
        }
        int index1 = indexes.at(key1);
        int index2 = indexes.at(key2);
        const auto& neighbors1 = nodes.at(index1).neighbors;
        return std::ranges::find(neighbors1, index2) != neighbors1.end();
    }

    void addNode(char transactionID) {
        indexes[transactionID] = nodes.size();
        nodes.emplace_back(transactionID);
    }

    void addEdge(char key1, char key2) {
        if(key1 != key2) {
            int index1 = indexes.at(key1);
            int index2 = indexes.at(key2);
            Node& node1 = nodes.at(index1);
            Node& node2 = nodes.at(index2);
            node1.neighbors.push_back(index2);
            node2.neighbors.push_back(index1);
        }
        else {
            int index = indexes.at(key1);
            Node& node = nodes.at(index);
            node.neighbors.push_back(index);
        }
    }

};

DependencyGraph dependencyGraph(const std::map<char, Transaction>& transactions) {
    DependencyGraph graph;
    for (auto const key1 : std::views::keys(transactions)) {
        for (auto const key2 : std::views::keys(transactions)) {
            const Transaction& first = transactions.at(key1);
            const Transaction& second = transactions.at(key2);
            if(!Transaction::areDependent(first, second) || graph.containsEdge(key1, key2)) {
                continue;
            }
            if(!graph.containsNode(key1)) {
                graph.addNode(key1);
            }
            if(!graph.containsNode(key2)) {
                graph.addNode(key2);
            }
            graph.addEdge(key1, key2);
        }
    }
    return graph;
}

DependencyGraph independencyGraph(const DependencyGraph& dependency, const std::set<char>& alphabet) {
    DependencyGraph graph;
    for(auto const key : std::views::keys(dependency.indexes)) {
        graph.addNode(key);
    }
    for(const auto from : alphabet) {
        for(const auto to : alphabet) {
            if(!dependency.containsEdge(from, to) && !graph.containsEdge(from, to)) {
                graph.addEdge(from, to);
            }
        }
    }
    return graph;
}

int main() {
    auto transactions = Transaction::getTransactions(INPUT_FILE, alphabet);
    // for (auto const& [key, val] : transactions) {
    //     std::cout << key << std::endl;
    //     val.print();
    //     std::cout << std::endl;
    // }
    auto dependency = dependencyGraph(transactions);
    dependency.printEdges();
    auto independency = independencyGraph(dependency, alphabet);
    independency.printEdges();
}
