#include "../include/algorithm.h"
#include <map>
#include <vector>
#include <ranges>
#include <deque>

void addEdgesFromDependency(DiekertGraph& graph, const DependencyGraph& dependency) {
    for(int i = 0; i < graph.getSize(); i++) {
        Node& node1 = graph.getNode(i);
        for(int j=i+1; j<graph.getSize(); j++) {
            const Node& node2 = graph.getNode(j);
            if(dependency.containsEdge(node1.transactionID, node2.transactionID)) {
                graph.addEdge(i, j);
            }
        }
    }
}

DiekertGraph reduceTransitively(DiekertGraph& graph, const std::string& word) {
    DiekertGraph transitive;
    transitive.addNodesFromWord(word);
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
                transitive.addEdge(start, node);
            }
        }
    }
    return transitive;
}

DiekertGraph createDiekertGraph(const std::string& word, const DependencyGraph& dependency) {
    DiekertGraph graph;
    graph.addNodesFromWord(word);
    addEdgesFromDependency(graph, dependency);
    return reduceTransitively(graph, word);
}

std::vector<int> getFoataMaxPaths(DiekertGraph& diekert) {
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
    return maxPaths;
}

void printFoataForm(DiekertGraph& diekert, const std::vector<int>& maxPaths) {
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
            std::cout << result << std::endl;
            return;
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