#include "tabusearch.hpp"

// Tabu Search Methods
TabuSearch::TabuSearch(int max_i, int tl_size, int lvl) {
    max_iterations = max_i;
    iteration = 0;
    tabulist = TabuList(tl_size);

    loglvl = lvl;
}

Solution TabuSearch::run(Scenario scenario, Solution initial) {
    Solution best(initial);
    while(true) {
        // Exit condition 1: cantidad de iteraciones maxima alcanzada
        if (iteration == max_iterations) {
            if(loglvl == 2) std::cout << "\nCONDICION DE SALIDA 1: Maxima cantidad de iteraciones alcanzada.\n";
            break;
        }
        if(loglvl == 2) std::cout << "[iteration " << iteration << "] ";

        // Obtener el vecindario actual aplicando un movimiento
        std::vector<Solution> neighborhood = initial.get_neighborhood();
        if(loglvl == 2) std::cout << "neighborhood: " << neighborhood.size() << " ";
        
        // Filtrar por soluciones que no se encuentren en la lista tabu
        auto end = std::remove_if(neighborhood.begin(), neighborhood.end(),
            [&](Solution option) {
                return tabulist.check(option);
            }
        );
        if(loglvl == 2) std::cout << "filtered: " << end - neighborhood.begin() << "\n";
        // Exit condition 2: no quedan mas opciones
        if (end - neighborhood.begin() == 0) {
            if(loglvl == 2) std::cout << "\nCONDICION DE SALIDA 2: No hay movimientos disponibles.\n";
            break;      
        }   

        // Evaluar soluciones 
        if(loglvl == 2) std::cout << "\tscores:";
        for(auto it = neighborhood.begin(); it < end; ++it) {
            scenario.evaluate(*it);
             if(loglvl == 2) std::cout << " " << it->get_score();
        }
        // Elegir el de mejor calidad (menor score)
        auto __best = std::min_element(std::begin(neighborhood), end, 
            [](Solution option1, Solution option2) {
                return option1.get_score() < option2.get_score(); 
            }
        );
        Solution iteration_best(*__best);
         if(loglvl == 2) std::cout << "\tbest: " << iteration_best.get_score() << "\n";
        // Se agrega solucion a la lista tabu
        tabulist.add_item(iteration_best);

        // Siempre se mantiene guardado al mejor
        if (iteration_best.get_score() < best.get_score()) {
            best = iteration_best;
        }
        ++iteration;
    }
    return best;
}

void TabuSearch::print() {
    std::cout << "Max iterarions: " << max_iterations << "\n";
    std::cout << "Length tabu list: " << tabulist.get_length() << "\n";
}

int TabuSearch::get_iterations() {
    return iteration;
}

int TabuSearch::get_tabulist_size() {
    return tabulist.get_size();
}



// Tabu List Methods
TabuList::TabuList() {
    size = 0;
}

TabuList::TabuList(unsigned int n) {
    size = n;
}

void TabuList::add_item(Solution solution) {
    struct item new_item = {solution};

    // Si la lista esta llena, eliminamos el ultimo item
    if (is_full()) {
        list.pop_front();
    }

    // Se agrega el nuevo item
    list.push_back(new_item);
}

bool TabuList::check(Solution solution) {
    for (auto i: list) {
        if(i.solution.equals(solution)) return true;
    }
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