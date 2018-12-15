#include "representation.hpp"

// Scenario Methods
Scenario::Scenario (std::string filepath, int lvl) {

    // debug
    loglvl = lvl;

    // Parse file    
    std::ifstream scenario_file(filepath);
    std::string aux_str;
    int aux_int;

    // Buses and yards
    scenario_file >> buses >> aux_str >> buses_capacity >> yards >> aux_str;
    for (int y = 0; y < yards; ++y) {
        scenario_file >> aux_int;
        for(int b=0; b < aux_int; ++b) {
            bus_yard.push_back(y);
        }
    }

    // Pickups and demand
    scenario_file >> pickups >> aux_str >> total_demand >> aux_str;
    for (int p = 0; p < pickups; ++p) {
        scenario_file >> aux_int;
        pickups_demand.push_back(aux_int);
    }

    // Shelters and capcity
    scenario_file >> shelters >> aux_str >> total_capacity >> aux_str;
    for (int s = 0; s < shelters; ++s) {
        scenario_file >> aux_int;
        shelters_capacity.push_back(aux_int);
    }
    
    // Yards to pickups distances
    for (int y = 0; y < yards; ++y) {
        scenario_file >> aux_int >> aux_str;
        std::vector<int> aux_vec;
        for (int p = 0; p < pickups; ++p) {
            scenario_file >> aux_int;
            aux_vec.push_back(aux_int);
        }
        dist_yard_to_pickup.push_back(aux_vec);
    }
        
    // Pickups to shelters distances
    for (int p = 0; p < pickups; ++p) {
        scenario_file >> aux_int >> aux_str;
        std::vector<int> aux_vec;
        for (int s = 0; s < shelters; ++s) {
            scenario_file >> aux_int;
            aux_vec.push_back(aux_int);
        }
        dist_pickup_to_shelter.push_back(aux_vec);
    }
}

void Scenario::print() {
    std::cout << "Buses: " << buses << "\nCapacidad: " << buses_capacity << "\n";
    std::cout << "Estaciones: " << yards << "\n";
    for (auto y: bus_yard) {
        std::cout << " " << y;
    }
    std::cout << "\n";

    std::cout << "Puntos de encuentro: " << pickups << "\n";
    for (auto i: pickups_demand) {
        std::cout << " " << i;
    }
    std::cout << " = " << total_demand << "\n";

    std::cout << "Refugios: " << shelters << "\n";
    for (auto i: shelters_capacity) {
        std::cout << " " << i;
    }
    std::cout << " = " << total_capacity << "\n";

    std::cout << "Estacion a P.E." << "\n";
    for (auto e: dist_yard_to_pickup) {
        for (auto p: e) {
            std::cout << p << " ";
        }
        std::cout << "\n";
    }

    std::cout << "P.E a Refugio." << "\n";
    for (auto p: dist_pickup_to_shelter) {
        for (auto s: p) {
            std::cout << s << " ";
        }
        std::cout << "\n";
    }
}

bool Scenario::is_feasible(Solution solution) {
    /* Restricciones
    / Todos deben ser evacuados, para esto la cantidad de viajes
    / aca P.E por la capacidad del bus, debe ser igual a 
    */
    // Cantidades de demanda y capacidad de P.E y Refigios respectivamente,
    // correspondiente al estado de la solucion actual.
    std::vector<int> demand(pickups_demand);
    std::vector<int> capacity(shelters_capacity);

    for(auto bus_i: solution.trips_table) {
        for(auto trip_t: bus_i) {
            // Actualizamos las demandas
            demand[trip_t.pickup] -= buses_capacity;
            capacity[trip_t.shelter] -= buses_capacity;
        }
    }

    // Toda la demanda satisfecha
    for(auto d: demand) {
        if(d != 0) return false;
    }
    // Refugios no superados en capacidad
    for(auto c: capacity) {
        if(c < 0) return false;
    }
    return true;
}

Solution Scenario::get_initial_solution() {
    Solution initial(buses);
    if (loglvl == 2) {std::cout << "COMIENZO DE ";}
    if (strcmp(METAHEURISTIC, "PROGRES") ==  0) {
        if (loglvl == 2) {std::cout << "PRObabilistic GREedy Search\n";} 
        __PROGRES(initial);
    } else if (strcmp(METAHEURISTIC, "PROGRESRANK") ==  0) {
        if (loglvl == 2) {std::cout << "PRObabilistic GREedy Search with Ranking\n";} 
        __PROGRESRANK(initial);
    } else {
        if (loglvl == 2) {std::cout << "Random Greedy Search\n";} 
        __GREEDYRANDOM(initial);
    }
    if (loglvl == 2) {std::cout << "FIN\n";}
    return initial;
}

std::vector<struct trip> Scenario::get_trips(int bus, Solution &solution) {
    // Cantidades de demanda y capacidad de P.E y Refigios respectivamente,
    // correspondiente al estado de la solucion actual.
    std::vector<int> demand(pickups_demand);
    std::vector<int> capacity(shelters_capacity);

    // Los viajes posibles son aquellos que parten desde un P.E con
    // demanda hacia Refugios que tengan capacidad aun despues de 
    // todos los viajes de la tabla.
    for(auto bus_i: solution.trips_table) {
        for(auto trip_t: bus_i) {
            // Actualizamos las demandas
            demand[trip_t.pickup] -= buses_capacity;
            capacity[trip_t.shelter] -= buses_capacity;
        }
    }
    // Revisamos agregamos todas las combinaciones posibles donde
    // los nodos aun tengan demanda/capacidad
    std::vector<struct trip> options_trips;
    for(int pickup = 0; pickup < pickups; ++pickup) {
        if (demand[pickup] > 0) {
            for (int shelter = 0; shelter < shelters; ++shelter) {
                if (capacity[shelter] > 0) {
                    options_trips.push_back({pickup, shelter});
                }
            }
        }
    }

    return options_trips;
}

std::vector<int> Scenario::calculate_buses_time(Solution &solution) {
    std::vector<int> buses_time(buses, 0);
    for(int bus = 0; bus < buses; ++bus) {
        std::vector<trip> bus_trips = solution.trips_table[bus];
        for(unsigned int t = 0; t < bus_trips.size(); ++t) {
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (t == 0) {
                buses_time[bus] += dist_yard_to_pickup[bus_yard[bus]][bus_trips[t].pickup];
            // en otro caso, se deba agregar el viaje de vuelta
            } else {
                buses_time[bus] += dist_pickup_to_shelter[bus_trips[t].pickup][bus_trips[t-1].shelter];
            } 
            // finalmente agregar el costo de ida
            buses_time[bus] += dist_pickup_to_shelter[bus_trips[t].pickup][bus_trips[t].shelter];
        }
    }
    return buses_time;
}

void Scenario::__PROGRES(Solution &solution) {
    // https://www.researchgate.net/publication/245280982_A_probabilistic_greedy_search_algorithm_for_combinatorial_optimisation_with_application_to_the_set_covering_problem
    int iterations = 0;
    // primero obtenenos los tiempos totales de cada bus actuales
    std::vector<int> buses_times = calculate_buses_time(solution);

    int bus;
    while(iterations++ < INITIAL_MAXITER ) {
        // Primero debemos hacer salir a todos los buses de las estaciones
        // Para esto vamos eligiendo cada bus en orden
        if (iterations <= buses || !RANDOM_BUS) {
            bus = (iterations-1)%buses;
        // Si RANDOM_BUS es verdadero. Luego de la primera iteracion
        // vamos eligiendo aleatoriamente
        } else {
            bus = rand() % buses;
        }
        
        // Luego de agregar cada viaje revisamos si es factible
        if (is_feasible(solution)) {
            int score = *std::max_element(buses_times.begin(), buses_times.end());
            solution.set_score(score, buses_times);
            return;
        }

        // Obtener opciones de viajes
        std::vector<struct  trip> bus_options = get_trips(bus, solution);
        if (loglvl == 2) {
            std::cout << "[Iteracion "<< iterations <<"]\nOpciones bus_" << bus << ":\n\t";
        }
        // Evaluar cada viaje en t
        std::vector<trip> bus_trips = solution.trips_table[bus];
        int trip_t = bus_trips.size();
        std::vector<std::tuple<int, struct trip>> evaluation_option;
        for (auto option: bus_options) { 
            int cost = 0;
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (trip_t == 0) {
                cost += dist_yard_to_pickup[bus_yard[bus]][option.pickup];
            // en otro caso, se deba agregar el viaje de vuelta            
            } else {
                cost += dist_pickup_to_shelter[option.pickup][bus_trips[trip_t-1].shelter];
            }
            // finalmente agregar el costo de ida
            cost += dist_pickup_to_shelter[option.pickup][option.shelter];

            evaluation_option.push_back(std::make_tuple(cost, option));
        }
        // Ordenar por calidad. La mejor calidad es aquella que aumenta 
        // menos el tiempo total de la evacuacion
        std::sort(std::begin(evaluation_option), std::end(evaluation_option), 
            [](std::tuple<int, struct trip> option1, std::tuple<int, struct trip> option2) {
                return std::get<0>(option1) < std::get<0>(option2); 
            }
        );

        // Se utiliza el procedimiento descrito en el paper para generar un vector
        // de probabilidades (o ruleta) a partir de todas las evaluaciones
        // Sumamos el total y buscamos el maximo
        std::vector<float> probability;
        int max_evaluation = -1;
        int sum_evaluation = 0;
        for (auto e:evaluation_option) {
            sum_evaluation += std::get<0>(e);
            if (max_evaluation < std::get<0>(e)) max_evaluation = std::get<0>(e);
        }
        // Calculamos la probabilidad para cada opcion
        float prob;
        for (auto e:evaluation_option) {
            prob = (max_evaluation - std::get<0>(e) + 1)/(evaluation_option.size()*(max_evaluation + 1.0) - sum_evaluation);
            probability.push_back(prob);
        }

        if (loglvl == 2) {
            int i = 0;
            for(auto e: evaluation_option) {
                std::cout << "(" << probability[i++] << ") " << std::get<0>(e) << ":(" << std::get<1>(e).pickup << " , "<< std::get<1>(e).shelter  << ") ";
            }
            std::cout << "\n";
        }

        // Seleccionar segun probabilidad
        if (evaluation_option.size() > 0) {
            std::tuple<int, struct trip> picked_option;
            
            double __rand = rand() / double(RAND_MAX);
            int picked_option_index = -1;
            if (loglvl == 2) {std::cout <<"RAND: "<< __rand<< "\t";}
            do {
                __rand = __rand - probability[++picked_option_index];
            } while(__rand > 0);
            picked_option = evaluation_option[picked_option_index];
            
            // Agregar a la solucion y actualizar tiempo total
            buses_times[bus] += std::get<0>(picked_option);
            solution.trips_table[bus].push_back(std::get<1>(picked_option));
            if (loglvl == 2) {
                std::cout << "Opcion elegida: "<< std::get<0>(picked_option) << " (";
                std::cout << std::get<1>(picked_option).pickup << " , ";
                std::cout << std::get<1>(picked_option).shelter << ")" ;
            }
        }

        if (loglvl == 2) {
            std::cout << "\nTiempos de cada bus: (";
            for(auto time: buses_times) {
                std::cout << time << " ";
            }
            std::cout << ")\n\n";
        }
    }
    int score = *std::max_element(buses_times.begin(), buses_times.end());
    solution.set_score(score, buses_times);
} 

void Scenario::__PROGRESRANK(Solution &solution) {
    // https://www.researchgate.net/publication/245280982_A_probabilistic_greedy_search_algorithm_for_combinatorial_optimisation_with_application_to_the_set_covering_problem
    int iterations = 0;
    // primero obtenenos los tiempos totales de cada bus actuales
    std::vector<int> buses_times = calculate_buses_time(solution);

    int bus;
    while(iterations++ < INITIAL_MAXITER ) {
        // Primero debemos hacer salir a todos los buses de las estaciones
        // Para esto vamos eligiendo cada bus en orden
        if (iterations <= buses || !RANDOM_BUS) {
            bus = (iterations-1)%buses;
        // Si RANDOM_BUS es verdadero. Luego de la primera iteracion
        // vamos eligiendo aleatoriamente
        } else {
            bus = rand() % buses;
        }
        
        // Luego de agregar cada viaje revisamos si es factible
        if (is_feasible(solution)) {
            int score = *std::max_element(buses_times.begin(), buses_times.end());
            solution.set_score(score, buses_times);
            return;
        }

        // Obtener opciones de viajes
        std::vector<struct  trip> bus_options = get_trips(bus, solution);
        if (loglvl == 2) {
            std::cout << "[Iteracion "<< iterations <<"]\nOpciones bus_" << bus << ":\n\t";
        }
        // Evaluar cada viaje en t
        std::vector<trip> bus_trips = solution.trips_table[bus];
        int trip_t = bus_trips.size();
        std::vector<std::tuple<int, struct trip>> evaluation_option;
        for (auto option: bus_options) { 
            int cost = 0;
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (trip_t == 0) {
                cost += dist_yard_to_pickup[bus_yard[bus]][option.pickup];
            // en otro caso, se deba agregar el viaje de vuelta            
            } else {
                cost += dist_pickup_to_shelter[option.pickup][bus_trips[trip_t-1].shelter];
            }
            // finalmente agregar el costo de ida
            cost += dist_pickup_to_shelter[option.pickup][option.shelter];

            evaluation_option.push_back(std::make_tuple(cost, option));
        }
        // Ordenar por calidad. La mejor calidad es aquella que aumenta 
        // menos el tiempo total de la evacuacion
        std::sort(std::begin(evaluation_option), std::end(evaluation_option), 
            [](std::tuple<int, struct trip> option1, std::tuple<int, struct trip> option2) {
                return std::get<0>(option1) < std::get<0>(option2); 
            }
        );

        // Se utiliza el procedimiento de ranking para calcular las probabilidades
        int ranking = (int)evaluation_option.size();
        float total_ranking = (ranking * (ranking + 1.0)) / 2.0;
        std::vector<float> probability;
        for (unsigned int r=0; r < evaluation_option.size(); ++r) {
            probability.push_back(((1.0* ranking)/total_ranking));
            --ranking;
        }
        if (loglvl == 2) {
            int i = 0;
            for(auto e: evaluation_option) {
                std::cout << "(" << probability[i++] << ") " << std::get<0>(e) << ":(" << std::get<1>(e).pickup << " , "<< std::get<1>(e).shelter  << ") ";
            }
            std::cout << "\n";
        }

        // Seleccionar segun probabilidad
        if (evaluation_option.size() > 0) {
            std::tuple<int, struct trip> picked_option;
            
            double __rand = rand() / double(RAND_MAX);
            int picked_option_index = -1;
            if (loglvl == 2) {std::cout <<"RAND: "<< __rand<< "\t";}
            do {
                __rand = __rand - probability[++picked_option_index];
            } while(__rand > 0);
            picked_option = evaluation_option[picked_option_index];
            
            // Agregar a la solucion y actualizar tiempo total
            buses_times[bus] += std::get<0>(picked_option);
            solution.trips_table[bus].push_back(std::get<1>(picked_option));
            if (loglvl == 2) {
                std::cout << "Opcion elegida: "<< std::get<0>(picked_option) << " (";
                std::cout << std::get<1>(picked_option).pickup << " , ";
                std::cout << std::get<1>(picked_option).shelter << ")" ;
            }
        }

        if (loglvl == 2) {
            std::cout << "\nTiempos de cada bus: (";
            for(auto time: buses_times) {
                std::cout << time << " ";
            }
            std::cout << ")\n\n";
        }
    }
    int score = *std::max_element(buses_times.begin(), buses_times.end());
    solution.set_score(score, buses_times);
} 


void Scenario::__GREEDYRANDOM(Solution &solution) {
    int iterations = 0;
    // primero obtenenos los tiempos totales de cada bus actuales
    std::vector<int> buses_times = calculate_buses_time(solution);

    int bus;
    while(iterations++ < INITIAL_MAXITER ) {
        // Primero debemos hacer salir a todos los buses de las estaciones
        // Para esto vamos eligiendo cada bus en orden
        if (iterations <= buses || !RANDOM_BUS) {
            bus = (iterations-1)%buses;
        // Si RANDOM_BUS es verdadero. Luego de la primera iteracion
        // vamos eligiendo aleatoriamente
        } else {
            bus = rand() % buses;
        }
        
        // Luego de agregar cada viaje revisamos si es factible
        if (is_feasible(solution)) {
            int score = *std::max_element(buses_times.begin(), buses_times.end());
            solution.set_score(score, buses_times);
            return;
        }

        // Obtener opciones de viajes
        std::vector<struct  trip> bus_options = get_trips(bus, solution);
        if (loglvl == 2) {
            std::cout << "[Iteracion "<< iterations <<"]\nOpciones bus_" << bus << ":\n\t";
        }
        // Evaluar cada viaje en t
        std::vector<trip> bus_trips = solution.trips_table[bus];
        int trip_t = bus_trips.size();
        std::vector<std::tuple<int, struct trip>> evaluation_option;
        for (auto option: bus_options) { 
            int cost = 0;
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (trip_t == 0) {
                cost += dist_yard_to_pickup[bus_yard[bus]][option.pickup];
            // en otro caso, se deba agregar el viaje de vuelta            
            } else {
                cost += dist_pickup_to_shelter[option.pickup][bus_trips[trip_t-1].shelter];
            }
                // finalmente agregar el costo de ida
            cost += dist_pickup_to_shelter[option.pickup][option.shelter];

            evaluation_option.push_back(std::make_tuple(cost, option));
        }
        // Ordenar por calidad. La mejor calidad es aquella que aumenta 
        // menos el tiempo total de la evacuacion
        std::sort(std::begin(evaluation_option), std::end(evaluation_option), 
            [](std::tuple<int, struct trip> option1, std::tuple<int, struct trip> option2) {
                return std::get<0>(option1) < std::get<0>(option2); 
            }
        );
        if (loglvl == 2) {
            for(auto e: evaluation_option) {
                std::cout << std::get<0>(e) << ":(" << std::get<1>(e).pickup << " , "<< std::get<1>(e).shelter  << ") ";
            }
            std::cout << "\n";
        }

        // Seleccionar aleatorio de una porcion
        if (evaluation_option.size() > 0) {
            int portion_index;
            std::tuple<int, struct trip> picked_option;
            if (evaluation_option.size() == 1) {
                picked_option = evaluation_option[0];
            }
            else {
                portion_index = GREEDYPORTION*evaluation_option.size();
                picked_option = evaluation_option[rand()%portion_index];
            }
            
            // Agregar a la solucion y actualizar tiempo total
            buses_times[bus] += std::get<0>(picked_option);
            solution.trips_table[bus].push_back(std::get<1>(picked_option));
            if (loglvl == 2) {
                std::cout << "Opcion elegida: "<< std::get<0>(picked_option);
                std::cout << std::get<1>(picked_option).pickup << " , ";
                std::cout << std::get<1>(picked_option).shelter << ")" ;
            }
        }

        if (loglvl == 2) {
            std::cout << "\nTiempos de cada bus: (";
            for(auto time: buses_times) {
                std::cout << time << " ";
            }
            std::cout << ")\n\n";
        }
    }
    int score = *std::max_element(buses_times.begin(), buses_times.end());
    solution.set_score(score, buses_times);
} 

void Scenario::evaluate(Solution &solution) {
    std::vector<int> buses_time = calculate_buses_time(solution);
    int score = *std::max_element(buses_time.begin(), buses_time.end());
    solution.set_score(score, buses_time);

}
void Scenario::evaluate(std::vector<Solution> &solutions) {
    for(auto solution: solutions) {
        evaluate(solution);
    }
}

void Scenario::write(std::ofstream &file, Solution &solution) {
    std::vector<int> shelter_final(shelters, 0); 
    std::vector<int> buses_times = solution.get_buses_times();
    int bottleneck_bus_value = *std::max_element(buses_times.begin(), buses_times.end());
    
    file << "Tabla de viajes:\n";
    file << "Bus | Tiempo | Estacion |  Ruta |\n";
    for(int bus = 0; bus < buses; ++bus) {
        if (buses_times[bus] == bottleneck_bus_value) {
            file << "(!)bus_" << bus + START_FROM_1 << " | "  << buses_times[bus] << " | "  << bus_yard[bus]<< " | ";
        } else {
            file << "bus_" << bus + START_FROM_1 << " | "  << buses_times[bus] << " | "  << bus_yard[bus]<< " | ";
        }
        for(auto t: solution.trips_table[bus]) {
            file <<  " ("  << t.pickup + START_FROM_1 << " , " << t.shelter + START_FROM_1 << ")";
            shelter_final[t.shelter] += buses_capacity;
        }
        file << "\n";
    }
    file << "Refugios:";
    for(auto s: shelter_final) {
        file << " " << s;
    }
    file << "\n";
}


// Solution Methods
Solution::Solution(int n_buses) {
    score = 0;
    buses = n_buses;

    for(int b=0; b < buses;++b) {
        std::vector<trip> bus_trips; 
        trips_table.push_back(bus_trips);
    }
}

Solution::Solution(const Solution &solution) {
    score = solution.score;
    buses = solution.buses;
    buses_times = solution.buses_times;
    trips_table = solution.trips_table;
    bus_yard = solution.bus_yard;
}


std::vector<Solution> Solution::get_neighborhood() {
    std::vector<Solution> neighborhood;
    int bottleneck_bus = std::distance(buses_times.begin(), std::max_element(buses_times.begin(), buses_times.end()));
    int dest_bus = rand()%buses;

    int max_trip_t = trips_table[dest_bus].size();
    if (dest_bus == bottleneck_bus) {
        --max_trip_t;
    }

    for(int trip_t=0; trip_t <= max_trip_t; ++trip_t) {
        neighborhood.push_back(swap_trip(bottleneck_bus, dest_bus, trip_t));
    }
    return neighborhood;
}

// Moviemientos
/* Swap_trip, se elige el bus cuello de botella como origen (bus_src)
/  y un bus destino(bus_dest), se toma el ultimo viaje del bus origen
/  y se inserta como el viaje t (trip_t) del bus destino.
/  El bus destino se elige aleatoriamente. Los buses origen y destino
/  pueden ser el mismo. 
*/
Solution Solution::swap_trip(int src_bus, int dest_bus, int trip_t) {
    Solution new_solution(*this);
    struct trip last_trip = new_solution.trips_table[src_bus].back();
    new_solution.trips_table[src_bus].pop_back();
    new_solution.trips_table[dest_bus].insert(new_solution.trips_table[dest_bus].begin()+trip_t, last_trip);
    return new_solution;
}
/*void Solution::swap_order(){}
void Solution::add_trip(){}
void Solution::del_trip(){}
*/
void Solution::print() {
    std::cout << "Tiempo Maximo: " << score << "\n";
    std::cout << "Buses: " << buses << "\n";
    std::cout << "Tabla de viajes:\n";
    for(int bus = 0; bus < buses; ++bus) {
        std::cout << "\tbus_" << bus + START_FROM_1 << ": ";
        for(auto t: trips_table[bus]) {
            std::cout <<  " ("  << t.pickup + START_FROM_1 << " , " << t.shelter + START_FROM_1 << ")";
        }
        std::cout << "\n";
    }
}

int Solution::get_buses() {
    return buses;
}

std::vector<int> Solution::get_buses_times(){
    return buses_times;
}

int Solution::get_score() {
    return score;
}

void Solution::set_score(int s, std::vector<int> buses_time) {
    score = s;
    buses_times = buses_time;
}

bool Solution::equals(const Solution& solution) {
    // Tienen la misma cantidad de buses
    if (this->buses != solution.buses) return false;

    // Tienen la misma cantidad de viajes por bus
    for(int bus=0; bus < this->buses; ++bus) {
        if (this->trips_table.size() != solution.trips_table.size()) return false;
    }

    // Tienen los mismos viajes por bus
    for(int bus=0; bus < this->buses; ++bus) {
        for(unsigned int trip=0; trip < this->trips_table[bus].size(); ++trip) {
            if ( this->trips_table[bus][trip].pickup != solution.trips_table[bus][trip].pickup ||
                    this->trips_table[bus][trip].shelter != solution.trips_table[bus][trip].shelter
            ) return false;
        }
    } 
    return true;
}