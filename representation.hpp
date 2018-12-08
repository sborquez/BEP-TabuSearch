#ifndef REPR
#define REPR 

#include <iostream>
#include <vector>
#include <fstream>


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
    Scenario(std::string filepath);
    ~Scenario(){};

    // evaluate es la 'funcion de evaluacion', esta se aplica a una solucion.
    void evaluate(Solution solution);
    void evaluate(std::vector<Solution> solutions);

    // get_initial_solution genera una solucion inicial factible para la heuristica.
    Solution get_initial_solution();

    // print muestra los parametros por pantalla.
    void print();

private:
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
};



class Solution
{
public:
    Solution(int n_buses);
    ~Solution(){};

    int get_score();
    void set_score(int s);

    std::vector<Solution> get_neighborhood();

    void print();
    void write(const char* log_file);

private:
    // Valor obtendio de la funcion de evaluacion.
    int score;

    // Cantidad de buses (filas de la tabla)
    int buses;

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

    // Moviemientos
    // TODO define all steps functions that it'll use
    void swap_buses();
    void swap_order();
    void add_trip();
    void del_trip();
};

#endif // !REPR