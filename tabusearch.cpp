#include "tabusearch.hpp"

TabuSearch::TabuSearch(int max_i, int tl_size, int lvl) {
    max_iterations = max_i;
    iteration = 0;
    tabulist = TabuList(tl_size);

    loglvl = lvl;
}

void TabuSearch::print() {
    std::cout << "Max iterarions: " << max_iterations << "\n";
    std::cout << "Length tabu list: " << tabulist.get_length() << "\n";
}

TabuList::TabuList() {
    size = 0;
}

TabuList::TabuList(int n) {
    size = n;
}

int TabuList::get_length() {
    return size;
}