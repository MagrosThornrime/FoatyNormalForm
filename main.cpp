#include <iostream>
#include <set>

#include "include/algorithm.h"

const std::string INPUT_FILE = "../transactions.txt";
const std::string WORD = "acdcfbbe";
const std::set ALPHABET = {'a', 'b', 'c', 'd', 'e', 'f'};
const std::string OUTPUT_FILE = "../graph.dot";

int main() {
    auto transactions = Transaction::getTransactions(INPUT_FILE, ALPHABET);
    auto dependency = dependencyGraph(transactions);
    std::cout << "D = ";
    dependency.printEdges();
    auto independency = independencyGraph(dependency, ALPHABET);
    std::cout << "I = ";
    independency.printEdges();

    auto diekert = createDiekertGraph(WORD, dependency);
    auto foataMaxPaths = getFoataMaxPaths(diekert);
    std::cout << "FNF = ";
    printFoataForm(diekert, foataMaxPaths);
    diekert.saveAsDot(OUTPUT_FILE);
}
