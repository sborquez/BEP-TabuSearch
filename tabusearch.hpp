#ifndef TABUSEARCH
#define TABUSEARCH

#include <iostream>
#include <deque>

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
/ Esta posee una lista de struct item, la cual puede almacenar solo
/ la informacion relevante de una solucion (por ejemplo el movimiento).
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
    TabuList(unsigned int n);
    ~TabuList(){};

    // add_item agrega una nueva solucion a la lista.
    void add_item(Solution solution);

    // check revisa si una solucion esta en la lista.
    bool check(Solution solution);

    // get_size retorna el tamano maximo de la lista.
    int get_size();

    // get_length retorna el tamano actual de la lista.
    int get_length();

private:
    unsigned int size;
    std::deque<item> list;

    bool is_full();
};


class TabuSearch
{
public:
    TabuSearch(int max_i, int tl_size, int lvl);
    ~TabuSearch(){};

    // run busca la mejor solucion dado un escenario y una solucion factible
    Solution run(Scenario scenario, Solution initial);

    // print Muestra en pantalla los parametros del algoritmo.
    void print();

    // get_iterations retorna la cantidad de iteraciones.
    int get_iterations();

    // get_tabulist_size retorna el largo de la lista tabu
    int get_tabulist_size();

private:
    int max_iterations;
    TabuList tabulist;

    // iteracion actual
    int iteration;

    // Debug 
    int loglvl;
};

#endif // !TABUSEARCH
