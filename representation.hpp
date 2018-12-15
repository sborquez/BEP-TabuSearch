#ifndef REPR
#define REPR 

#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <fstream>
#include <string.h> 

#define INITIAL_MAXITER 1000
#define GREEDYPORTION 0.5
#define METAHEURISTIC "PROGRESRANK"
//#define METAHEURISTIC "RANDOMGREEDY"
//#define METAHEURISTIC "PROGRES"
#define USEPROGRES true
#define RANDOM_BUS false
#define START_FROM_1 false

/*
/ Scenario representa a una 'instancia' del problema. Contiene las diferentes
/ variables de un escenario en particular. Scenario se encarga de generar una
/ solucion inicial y de evaluar las soluciones.
*/
class Scenario;

/*
/ Solution es la implementacion de la 'representacion' para el problema BEP. Esta
/ se encarga de generar el vecindario definido por el 'movimiento' implementado.
*/
class Solution;

/*
/ trip es parte de la 'representacion', es un viaje de un bus entre un punto de 
encuentro (pickup) y un refugio (shelter).
*/
struct trip {
    int pickup;
    int shelter;
};


class Scenario
{
public:
    Scenario(std::string filepath, int loglvl);
    ~Scenario(){};

    // evaluate es la 'funcion de evaluacion', esta se aplica a una solucion.
    void evaluate(Solution &solution);
    void evaluate(std::vector<Solution> &solutions);

    // get_initial_solution genera una solucion inicial factible utilizando la 
    // heuristica de Random Greedy Choice
    Solution get_initial_solution();

    // is_feasible revisa si la solucion dada es factible, cumple las restricciones 
    bool is_feasible(Solution solution);

    // print muestra los parametros por pantalla.
    void print();
    // write solution
    void write(std::ofstream &file, Solution &ssolution);
private:
    // debug
    int loglvl;

    // Archivo de la instancia
    char* filepath;

    // Parametros de la instancia
    int buses;
    int shelters;
    int yards;
    int pickups;

    int total_demand;
    int total_capacity;
    int buses_capacity;
    std::vector<int> shelters_capacity;
    std::vector<int> pickups_demand;

    // Lista de buses por estacion
    std::vector<int> bus_yard;

    // Distancia entre cada estacion a los puntos de encuetro
    std::vector<std::vector<int>> dist_yard_to_pickup;

    // Distancia de cada punto de encuetro a los refugios
    std::vector<std::vector<int>> dist_pickup_to_shelter;

    // Retorna las opciones de viajes que tiene el bus en el estado de la solucion.
    std::vector<struct trip> get_trips(int bus, Solution &solution);
    
    // Retorna el tiempo de cada bus en el estado de la solucion.
    std::vector<int> calculate_buses_time(Solution &solution);

    // MetaHeuristica para la solucion inicial
    void __PROGRES(Solution &Solution);
    void __PROGRESRANK(Solution &solution);
    void __GREEDYRANDOM(Solution &solution);
};



class Solution
{
public:
    /*
    / Representacion de la solucion, como una tabla de viajes
    / por bus. Cada fila son los viajes realizados por un bus.
    / Se obvia el primer viaje desde la estacion (yard) y los
    / viajes de vuelta desde un refugio (shelter) a un punto
    / de encuentro (pickup). De esta forma solo almacenamos
    / los viajes de ida desde los puntos de encuentro (pickup)
    / a los refugios.
    */
    std::vector<std::vector<struct trip>> trips_table;

    Solution(int n_buses);
    Solution(const Solution &solution);
    ~Solution(){};

    int get_buses();
    std::vector<int> get_buses_times();
    int get_score();
    void set_score(int s, std::vector<int> buses_times);

    std::vector<Solution> get_neighborhood();

    void print();
    
    bool equals(const Solution& solution);

private:
    // Valor obtendio de la funcion de evaluacion.
    int score;
    std::vector<int> buses_times;

    // Cantidad de buses (filas de la tabla)
    int buses;
    std::vector<int> bus_yard;

    // Moviemientos
    Solution swap_trip(int bus, int trip1, int trip2);
    /*
    void swap_order();
    void add_trip();
    void del_trip();
    */
};

#endif // !REPR