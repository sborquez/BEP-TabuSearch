#ifndef TABUSEARCH
#define TABUSEARCH

#include <queue>

#include "representation.hpp"

/*
/ TabuSearch es la tecnica a aplicar, posee sus parametros de
/ largo de lista tabu y la cantidad maxima de iteraciones
*/
class TabuSearch;

/*
/ TabuList es un Queue de tamano fijo, este almacena las soluciones
/ ya visitadas para no tener que volver a elegirlas. Tiene un 
/ tamano fijo, elimina al item mas viejo para agregar uno nuevo.
*/
class TabuList;

/*
/ item es un item de la lista tabu, este almacena la informacion
/ necesaria para identificar a cada solucion
*/
struct item {
    Solution solution;
};


class TabuSearch
{
public:
    TabuSearch(int max_iterations, int tabu_list_size, int loglvl);
    ~TabuSearch();

    Solution run(Scenario scenario);

private:
    int max_iterations;
    int tabu_list_size
    ;
    // iteracion actual
    int iteration;

    // Debug 
    int loglvl;

    // Revisa si ya se alcanzo una solucion optima o el limite de iteraciones
    bool exit_condition();
};

class TabuList
{
public:
    TabuList(int size);
    ~TabuList();

    // add a new solution to the list
    void add_item(Solution solution);

    // check if solution is not in the list
    bool check(Solution solution);

private:
    int size;
    std::queue<item> list;
};

#endif // !TABUSEARCH
