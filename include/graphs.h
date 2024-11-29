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

    // save graph to a file in .dot format
    void saveAsDot(const std::string& filename) const;

    // print each Node and its neighbors
    void printNodes() const;
};

struct DependencyGraph : Graph {
    std::map<char, int> indexes;

    // print all edges in the graph
    void printEdges() const;

    // check if there exists a Node with given transaction id
    bool containsNode(char transactionID) const;

    // check if there exists an edge of given Nodes
    bool containsEdge(char key1, char key2) const;

    // add an empty Node with given transaction id
    void addNode(char transactionID);

    // add and edge between two given Nodes
    void addEdge(char key1, char key2);
};

struct DiekertGraph : Graph {
    // add an edge between two given Nodes
    void addEdge(int from, int to);

    // get number of Nodes
    int getSize() const;

    // get a reference to Node with given index
    Node& getNode(int index);

    // add an empty Node with given transaction id
    void addNode(char transactionID);

    // check if there is an edge of two given Nodes
    bool containsEdge(int from, int to) const;

    // create empty Nodes for each transaction in word
    void addNodesFromWord(const std::string& word);
};