#pragma once
#include "../include/graphs.h"
#include "../include/transaction.h"

// using the dependency graph, connect each Node in the Diekert graph
void addEdgesFromDependency(DiekertGraph& graph, const DependencyGraph& dependency);

// apply the transitive reduction to the Diekert graph
DiekertGraph reduceTransitively(DiekertGraph& graph, const std::string& word);

// create a Diekert graph from a word and a dependency graph
DiekertGraph createDiekertGraph(const std::string& word, const DependencyGraph& dependency);

// apply a modified BFS to find Foata levels for each Node in Diekert graph
std::vector<int> getFoataMaxPaths(DiekertGraph& diekert);

// print Foata Normal Form using the vector of levels
void printFoataForm(DiekertGraph& diekert, const std::vector<int>& maxPaths);

// create a dependency graph
DependencyGraph dependencyGraph(const std::map<char, Transaction>& transactions);

// create an independency graph
DependencyGraph independencyGraph(const DependencyGraph& dependency, const std::set<char>& alphabet);