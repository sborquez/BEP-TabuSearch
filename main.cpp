#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

#include "representation.hpp"
#include "tabusearch.hpp"

void print_usage()
{
    std::cout << "Uso\n";
    std::cout << "bep -i <ruta/de/instancia> -o <ruta/de/log> [-n <iteraciones>] [-l <largo_lista_tabu>] [-v <nivel>]\n\n";
    std::cout << "Donde:\n";
    std::cout << "\truta/de/instancia: Es la ruta al archivo de la instancia que se va a probar." << '\n';
    std::cout << "\truta/de/log: Es la ruta del archivo output para la instancia." << '\n';
    std::cout << "\titeraciones (default=100): Es la cantidad máxima de iteraciones para el algoritmo." << '\n';
    std::cout << "\tlargo_lista_tabu (default=6): Es el largo de la lista tabú." << '\n';
    std::cout << "\tnivel(default=0): Es el nivel de output por consola. Donde 0 es el normal, 1 incluye información por iteración y 2 incluye información extra por cada paso." << '\n';
}


int main( int argc, char* argv[] )
{
    // Lectura de parametros
    if (argc < 5 || 11 < argc) {
        print_usage();
        return 1;
    }
    std::string input_file = ""; 
    std::string output_file = "";
    int max_iterations = 100;
    int tabu_list_len = 6 ;
    int loglvl = 0;
    
    int c ;
    while((c = getopt(argc, argv, "i:o:n:l:v:")) != -1) 
    {
        switch(c)
        {
            case 'i':
                if (optarg) input_file = optarg;
                break;
            case 'o':
                if(optarg) output_file = optarg;
                break;
            case 'n':
                if(optarg) max_iterations = std::atoi(optarg);
                break;
            case 'l':
                if(optarg) tabu_list_len = std::atoi(optarg);
                break;
            case 'v':
                if(optarg) loglvl = std::atoi(optarg);
                break;
        }
    }
    
    if (loglvl == 2) {
        std::cout << "Parametros del programa\n";
        std::cout << "Input file: " << input_file << "\n";
        std::cout << "Outpyt file: " << output_file << '\n';
        std::cout << "Max iterations: " << max_iterations << '\n';
        std::cout << "Tabu list length: " << tabu_list_len << '\n';
        std::cout << "logging level: " << loglvl << "\n\n";
    
        std::cout << "Cargando Scenario \n";
    }    

    // Cargar el escenario de la instancia
    Scenario scenario (input_file, loglvl);
    if (loglvl == 2) {
        scenario.print();
    }

    // Creamos un nuevo Tabu Search con los parametros dados
    TabuSearch solver (max_iterations, tabu_list_len, loglvl);
    if (loglvl == 2) {
        std::cout << "\nTabu Search:\n";
        solver.print();
        std::cout << "\nBuscando solucion inicial\n";
    }

    // Buscamos una solucion inicial para entregarsela a Tabu Search.
    Solution initial = scenario.get_initial_solution();
    if (loglvl == 2) {
        std::cout << "\nSolucion inicial\n";
        initial.print();

        std::cout << "\nComenzando TabuSearch\n";
    }

    // Hacemos correr el algoritmo con el escenario y solucion inicial.
    Solution best = solver.run(scenario, initial);
    if (loglvl == 2) {
        std::cout << "\nMejor solucion\n";
        best.print();
    }
    return 0;
}