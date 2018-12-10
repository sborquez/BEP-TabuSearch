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

Solution Scenario::get_initial_solution() {
    return get_initial_solution("GREEDYRANDOM");
}

Solution Scenario::get_initial_solution(std::string metaheuristic) {
    Solution initial(buses);
    if (loglvl == 2) {std::cout << "START " << metaheuristic << "\n";}
    if (metaheuristic.compare("GRASP") == 0) {
        __GRASP(initial);
    } else if (metaheuristic.compare("GREEDYRANDOM") == 0) {
        __GREEDYRANDOM(initial);
    }
    if (loglvl == 2) {std::cout << "END " << metaheuristic << "\n";}
    return initial;
}

void Scenario::__GRASP(Solution &solution) {
    // TODO generate an initial solution using Greedy randomized adaptive
    // search procedure. This require Hill Climbing algorithm (__HILLCLIMBING)
} 

void Scenario::__GREEDYRANDOM(Solution &solution) {
    int iterations = 0;
    // primero obtenenos los tiempos totales de cada bus actuales
    std::vector<int> total_time(buses, 0);
    for(int bus = 0; bus < buses; ++bus) {
        std::vector<trip> bus_trips = solution.trips_table[bus];
        for(int t = 0; t < bus_trips.size(); ++t) {
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (t == 0) {
                total_time[bus] += dist_yard_to_pickup[bus_yard[bus]][bus_trips[t].pickup];
            // en otro caso, se deba agregar el viaje de vuelta
            } else {
                total_time[bus] += dist_pickup_to_shelter[bus_trips[t].pickup][bus_trips[t-1].shelter];
            } 
            // finalmente agregar el costo de ida
            total_time[bus] += dist_pickup_to_shelter[bus_trips[t].pickup][bus_trips[t].shelter];
        }
    }
    

    while(iterations++ < GREEDYMAXITER ) {
        // Primero debemos hacer salir a todos los buses de las estaciones
        int b;
        if (iterations < buses || !GREEDYRANDOMBUS) {
            b = (iterations-1)%buses;
        // Luego se eligen aleatoria mente
        } else {
            b = rand() % buses;
        }
        
        // Luego de agregar cada viaje revisamos si es factible
        if (is_feasible(solution)) {
            int score = *std::max_element(total_time.begin(), total_time.end());
            solution.set_score(score);
            return;
        }
        // Obtener opciones de viajes
        std::vector<struct  trip> bus_options = get_trips(b, solution);
        if (loglvl == 2) {
            std::cout << "["<< iterations <<" bus_" << b << "]:";
            for(auto t: bus_options) {
                std::cout << " [" << t.pickup << "->" << t.shelter << "]";
            }
            std::cout << "\n";
        }
        // Evaluar cada viaje en t
        std::vector<trip> bus_trips = solution.trips_table[b];
        int bus_t = bus_trips.size();
        std::vector<std::tuple<int, struct trip>> evaluation_option;
        for (auto option: bus_options) {
            int cost = 0;
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (bus_t == 0) {
                cost += dist_yard_to_pickup[bus_yard[b]][option.pickup];
            // en otro caso, se deba agregar el viaje de vuelta            
            } else {
                cost += dist_pickup_to_shelter[option.pickup][bus_trips[bus_t-1].shelter];
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
            std::cout << "evaluations:";
            for(auto e: evaluation_option) {
                std::cout << " [" << std::get<1>(e).pickup << "->"<< std::get<1>(e).shelter  << "]:" << std::get<0>(e);
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
            total_time[b] += std::get<0>(picked_option);
            solution.trips_table[b].push_back(std::get<1>(picked_option));
            if (loglvl == 2) {
                std::cout << "Picked option: [" << std::get<1>(picked_option).pickup << "->";
                std::cout << std::get<1>(picked_option).shelter << "]:" << std::get<0>(picked_option);
            }
        }

        if (loglvl == 2) {
            std::cout << "\ntotal times:";
            for(auto time: total_time) {
                std::cout << " " << time;
            }
            std::cout << "\n";
        }
    }
    int score = *std::max_element(total_time.begin(), total_time.end());
    solution.set_score(score);
} 

void Scenario::__HILLCLIMBING(Solution &solution) {
    //TODO use an initial (greedy) solution and  
}

void Scenario::evaluate(Solution solution) {
    std::vector<int> total_time(buses, 0);
    for(int bus = 0; bus < buses; ++bus) {
        std::vector<trip> bus_trips = solution.trips_table[bus];
        for(int t = 0; t < bus_trips.size(); ++t) {
            // Si es el primer viaje, se debe agregar el costo de 
            // la estacion al P.E
            if (t == 0) {
                total_time[bus] += dist_yard_to_pickup[bus_yard[bus]][bus_trips[t].pickup];
            // en otro caso, se deba agregar el viaje de vuelta
            } else {
                total_time[bus] += dist_pickup_to_shelter[bus_trips[t].pickup][bus_trips[t-1].shelter];
            } 
            // finalmente agregar el costo de ida
            total_time[bus] += dist_pickup_to_shelter[bus_trips[t].pickup][bus_trips[t].shelter];
        }
    }
    int score = *std::max_element(total_time.begin(), total_time.end());
    solution.set_score(score);

}
void Scenario::evaluate(std::vector<Solution> solutions) {
    for(auto solution: solutions) {
        evaluate(solution);
    }
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

std::vector<Solution> Solution::get_neighborhood() {
    // TODO generate a list of solutions using some step function.
    std::vector<Solution> neighborhood;
    return neighborhood;
}

// Moviemientos
// TODO declare all steps functions that it'll use
void Solution::swap_buses(){}
void Solution::swap_order(){}
void Solution::add_trip(){}
void Solution::del_trip(){}


void Solution::print() {
    std::cout << "Score: " << score << "\n";
    std::cout << "Buses: " << buses << "\n";
    std::cout << "Trip Table:\n";
    for(int bus = 0; bus < buses; ++bus) {
        std::cout << "bus_" << bus+1 << ": ";
        for(auto t: trips_table[bus]) {
            std::cout <<  " ("  << t.pickup + 1 << " , " << t.shelter + 1 << ")";
        }
        std::cout << "\n";
    }
}

void Solution::write(const char* log_file) {
    // TODO write solution to file
}

int Solution::get_score() {
    return score;
}

void Solution::set_score(int s) {
    score = s;
}