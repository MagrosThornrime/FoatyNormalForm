#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <map>

struct Node {
    char transactionID;
    std::vector<int> neighbors;
};

struct Graph {
    std::vector<Node> nodes;

    void saveAsDot(const std::string& filename) const;

    void printNodes() const;
};

struct DependencyGraph : Graph {
    std::map<char, int> indexes;

    void printEdges() const;

    bool containsNode(char transactionID) const;

    bool containsEdge(char key1, char key2) const;

    void addNode(char transactionID);

    void addEdge(char key1, char key2);
};

struct DiekertGraph : Graph {
    void addEdge(int from, int to);

    int getSize() const;

    Node& getNode(int index);

    void addNode(char transactionID);

    bool containsEdge(int from, int to) const;

    void addNodesFromWord(const std::string& word);
};