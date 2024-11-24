#pragma once
#include "../include/graphs.h"
#include "../include/transaction.h"

void addEdgesFromDependency(DiekertGraph& graph, const DependencyGraph& dependency);

DiekertGraph reduceTransitively(DiekertGraph& graph, const std::string& word);

DiekertGraph createDiekertGraph(const std::string& word, const DependencyGraph& dependency);

std::vector<int> getFoataMaxPaths(DiekertGraph& diekert);

void printFoataForm(DiekertGraph& diekert, const std::vector<int>& maxPaths);

DependencyGraph dependencyGraph(const std::map<char, Transaction>& transactions);

DependencyGraph independencyGraph(const DependencyGraph& dependency, const std::set<char>& alphabet);