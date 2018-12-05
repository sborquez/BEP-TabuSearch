#ifndef REPR
#define REPR 

#include <vector>

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
    Scenario(const char* file_path);
    ~Scenario();

    // evaluate es la 'funcion de evaluacion', esta se aplica a una solucion.
    void evaluate(Solution solution);
    void evaluate(std::vector<Solution> solutions);

    // get_initial_solution genera una solucion inicial factible para la heuristica.
    Solution get_initial_solution();

private:
    // Archivo de la instancia
    char* file_path;

    // Parametros de la instancia
    int buses;
    int shelters;
    int yards;
    int pickups;

    int total_demand;
    int buses_capacity;
    std::vector<int> shelters_capacity;
    std::vector<int> pickups_demand;

    // Lista de estaciones correspondientes al bus (indice)
    std::vector<int> bus_yard;

    // Distancia entre cada estacion a los puntos de encuetro
    std::vector<std::vector<int>> dist_yard_to_pickup;

    // Distancia de cada punto de encuetro a los refugios
    std::vector<std::vector<int>> dist_pickup_to_shelter;
};



class Solution
{
public:
    Solution();
    ~Solution();

    int get_score();
    void set_score(int score);

    std::vector<Solution> get_neighborhood();

    void print();
    void write(const char* log_file);

private:
    // Valor obtendio de la funcion de evaluacion.
    int score;

    /*
    / Representacion de la solucion, como una tabla de viajes
    / por bus. Cada fila son los viajes realizados por un bus.
    / Se obvia el primer viaje desde la estacion (yard) y los
    / viajes de vuelta desde un refugio (shelter) a un punto
    / de encuentro (pickup).
    */
    std::vector<std::vector<struct trip>> trips_table;
};

#endif // !REPR