#ifndef TABUSEARCH
#define TABUSEARCH

#include <iostream>
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

class TabuList
{
public:
    TabuList();
    TabuList(int n);
    ~TabuList(){};

    // add_item agrega una nueva solucion a la lista.
    void add_item(Solution solution);

    // check revisa si una solucion esta en la lista.
    bool check(Solution solution);

    // get_length retorna el tamano maximo de la lista.
    int get_length();

private:
    int size;
    std::queue<item> list;
};


class TabuSearch
{
public:
    TabuSearch(int max_i, int tl_size, int lvl);
    ~TabuSearch(){};

    Solution run(Scenario scenario, Solution initial);

    // print Muestra en pantalla los parametros del algoritmo.
    void print();

private:
    int max_iterations;
    TabuList tabulist;

    // iteracion actual
    int iteration;

    // Debug 
    int loglvl;

    // Revisa si ya se alcanzo una solucion optima o el limite de iteraciones
    bool exit_condition();
};

#endif // !TABUSEARCH
