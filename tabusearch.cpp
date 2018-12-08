#include "tabusearch.hpp"

// Tabu Search Methods
TabuSearch::TabuSearch(int max_i, int tl_size, int lvl) {
    max_iterations = max_i;
    iteration = 0;
    tabulist = TabuList(tl_size);

    loglvl = lvl;
}

Solution TabuSearch::run(Scenario scenario, Solution initial) {
    // TODO Finish this algorithm
    return initial;
}

void TabuSearch::print() {
    std::cout << "Max iterarions: " << max_iterations << "\n";
    std::cout << "Length tabu list: " << tabulist.get_length() << "\n";
}

bool TabuSearch::exit_condition() {
    // TODO evaluate exit condition for Tabu Search
    return false;
}


// Tabu List Methods
TabuList::TabuList() {
    size = 0;
}

TabuList::TabuList(int n) {
    size = n;
}

void TabuList::add_item(Solution solution) {
    struct item new_item = {solution};

    // Si la lista esta llena, eliminamos el ultimo item
    if (is_full()) {
        list.pop();
    }

    // Se agrega el nuevo item
    list.push(new_item);
}

bool TabuListcheck(Solution solution) {
    // TODO check if a solution is in the list
    return false;
}

int TabuList::get_length() {
    return list.size();
}

int TabuList::get_size() {
    return size;
}

bool TabuList::is_full() {
    return list.size() == size;
}