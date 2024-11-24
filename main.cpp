#include <iostream>
#include <map>
#include <vector>
#include <ranges>
#include <deque>

#include "include/transaction.h"

const std::string INPUT_FILE = "../transactions.txt";
const std::string WORD = "acdcfbbe";
const std::set ALPHABET = {'a', 'b', 'c', 'd', 'e', 'f'};
const std::string OUTPUT_FILE = "../graph.dot";

struct Node {
    char transactionID;
    std::vector<int> neighbors;

};

struct Graph {
    std::vector<Node> nodes;

    void saveAsDot(const std::string& filename) const {
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

    void printNodes() {
        for (const auto& node : nodes) {
            std::cout << node.transactionID << std::endl;
            for (const auto& neighbor : node.neighbors) {
                std::cout << nodes.at(neighbor).transactionID << " ";
            }
            std::cout << std::endl << std::endl;
        }
    }
};

struct DependencyGraph : Graph {
    std::map<char, int> indexes;

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

struct DiekertGraph : Graph {
    void addEdge(int from, int to) {
        Node& node1 = nodes.at(from);
        node1.neighbors.push_back(to);
    }

    int getSize() const {
        return nodes.size();
    }

    Node& getNode(int index) {
        return nodes.at(index);
    }

    void addNode(char transactionID) {
        nodes.emplace_back(transactionID);
    }

    bool hasEdge(int from, int to) const {
        const Node& nodeFrom = nodes.at(from);
        return std::ranges::find(nodeFrom.neighbors, to) != nodeFrom.neighbors.end();
    }

};

DiekertGraph diekertGraph(const std::string& word, const DependencyGraph& dependency) {
    DiekertGraph graph;
    // initialize empty graph
    for(const auto letter : word) {
        graph.addNode(letter);
    }
    // add edges for each edge in dependency graph
    for(int i = 0; i < graph.getSize(); i++) {
        Node& node1 = graph.getNode(i);
        for(int j=i+1; j<graph.getSize(); j++) {
            const Node& node2 = graph.getNode(j);
            if(dependency.containsEdge(node1.transactionID, node2.transactionID)) {
                graph.addEdge(i, j);
            }
        }
    }
    DiekertGraph graph2;
    // initialize empty graph2
    for(const auto letter : word) {
        graph2.addNode(letter);
    }

    // transitive reduction
    for(int start=0; start<graph.getSize(); start++) {
        const Node& startNode = graph.getNode(start);
        std::deque<int> stack;
        std::deque<int> parents;
        std::vector maxLengths(graph.nodes.size(), 0);
        for(int neighbor : startNode.neighbors) {
            stack.push_back(neighbor);
            parents.push_back(start);
        }
        while(!stack.empty()) {
            int next = stack.back();
            stack.pop_back();
            int parent = parents.back();
            parents.pop_back();
            maxLengths[next] = std::max(maxLengths[next], maxLengths[parent] + 1);
            const Node& nextNode = graph.getNode(next);
            for(auto neighbor : nextNode.neighbors) {
                stack.push_back(neighbor);
                parents.push_back(next);
            }
        }
        for(int node=start+1; node<graph.getSize(); node++) {
            if(maxLengths[node] == 1) {
                graph2.addEdge(start, node);
            }
        }
    }

    return graph2;
}

std::string foataForm(DiekertGraph& diekert) {
    std::vector maxPaths(diekert.getSize(), 0);
    for(int start=0; start<diekert.getSize(); start++) {
        if(maxPaths[start] > 0) {
            continue;
        }
        std::deque<int> queue;
        std::deque<int> parents;
        maxPaths[start] = 1;
        const Node& startNode = diekert.getNode(start);
        for (auto neighbor : startNode.neighbors) {
            queue.push_back(neighbor);
            parents.push_back(start);
        }
        while(!queue.empty()) {
            int next = queue.front();
            queue.pop_front();
            int parent = parents.front();
            parents.pop_front();
            maxPaths[next] = std::max(maxPaths[next], maxPaths[parent] + 1);
            const Node& nextNode = diekert.getNode(next);
            for(auto neighbor : nextNode.neighbors) {
                queue.push_back(neighbor);
                parents.push_back(next);
            }
        }
    }
    std::string result;
    int levelNumber = 1;
    while(true) {
        std::string currentLevel;
        for(int i=0; i<diekert.getSize(); i++) {
            if(maxPaths[i] == levelNumber) {
                currentLevel += diekert.getNode(i).transactionID;
            }
        }
        if(currentLevel.empty()) {
            return result;
        }
        result += "(" + currentLevel + ")";
        levelNumber++;
    }
}

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
    auto transactions = Transaction::getTransactions(INPUT_FILE, ALPHABET);
    auto dependency = dependencyGraph(transactions);
    dependency.printEdges();
    auto independency = independencyGraph(dependency, ALPHABET);
    independency.printEdges();

    auto diekert = diekertGraph(WORD, dependency);
    std::cout << foataForm(diekert) << std::endl;
    diekert.saveAsDot(OUTPUT_FILE);
}
