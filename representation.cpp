#include "representation.hpp"

Scenario::Scenario (std::string filepath) {

    // Parse file    
    std::ifstream scenario_file(filepath);
    std::string aux_str;
    int aux_int;

    // Buses and yards
    scenario_file >> buses >> aux_str >> buses_capacity >> yards >> aux_str;
    for (int y = 0; y < yards; ++y) {
        scenario_file >> aux_int;
        bus_yard.push_back(aux_int);
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
    for (auto i: bus_yard) {
        std::cout << " " << i;
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

Solution Scenario::get_initial_solution() {
    Solution solution(buses);
    // TODO
    return solution;
}

Solution::Solution(int n_buses) {
    score = 0;
    buses = n_buses;

    for(int b=0; b < buses;++b) {
        std::vector<trip> bus_trips; 
        trips_table.push_back(bus_trips);
    }
}

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

int Solution::get_score() {
    return score;
}

void Solution::set_score(int s) {
    score = s;
}