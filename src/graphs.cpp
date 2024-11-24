#include "../include/graphs.h"
#include <fstream>

void Graph::saveAsDot(const std::string& filename) const {
    std::string fileContent = "digraph g{\n";
    for(int i=0; i < nodes.size(); i++) {
        const Node& node = nodes.at(i);
        for(auto neighbor : node.neighbors) {
            fileContent += std::to_string(i) + " -> " + std::to_string(neighbor) + "\n";
        }
    }
    for(int i=0; i < nodes.size(); i++) {
        const Node& node = nodes.at(i);
        fileContent += std::to_string(i) + "[label=" + node.transactionID + "]\n";
    }
    fileContent += "}";
    std::ofstream file(filename);
    file << fileContent;
    file.close();
}

void Graph::printNodes() const {
    for (const auto& node : nodes) {
        std::cout << node.transactionID << std::endl;
        for (const auto& neighbor : node.neighbors) {
            std::cout << nodes.at(neighbor).transactionID << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

void DependencyGraph::printEdges() const {
    std::cout << "{";
    for (const auto& node : nodes) {
        for(const auto& neighbor : node.neighbors) {
            char neighborID = nodes.at(neighbor).transactionID;
            std::cout << "(" << node.transactionID << "," << neighborID << "), ";
        }
    }
    std::cout << "}" << std::endl;
}

bool DependencyGraph::containsNode(char transactionID) const {
    return indexes.contains(transactionID);
}

bool DependencyGraph::containsEdge(char key1, char key2) const {
    if(!indexes.contains(key1) || !indexes.contains(key2)) {
        return false;
    }
    int index1 = indexes.at(key1);
    int index2 = indexes.at(key2);
    const auto& neighbors1 = nodes.at(index1).neighbors;
    return std::ranges::find(neighbors1, index2) != neighbors1.end();
}

void DependencyGraph::addNode(char transactionID) {
    indexes[transactionID] = nodes.size();
    nodes.emplace_back(transactionID);
}

void DependencyGraph::addEdge(char key1, char key2) {
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

void DiekertGraph::addEdge(int from, int to) {
    Node& node1 = nodes.at(from);
    node1.neighbors.push_back(to);
}

int DiekertGraph::getSize() const {
    return nodes.size();
}

Node& DiekertGraph::getNode(int index) {
    return nodes.at(index);
}

void DiekertGraph::addNode(char transactionID) {
    nodes.emplace_back(transactionID);
}

bool DiekertGraph::containsEdge(int from, int to) const {
    const Node& nodeFrom = nodes.at(from);
    return std::ranges::find(nodeFrom.neighbors, to) != nodeFrom.neighbors.end();
}

void DiekertGraph::addNodesFromWord(const std::string &word) {
    for(const auto letter : word) {
        addNode(letter);
    }
}
