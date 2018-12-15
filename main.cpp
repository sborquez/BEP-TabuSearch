#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>

// Medir tiempo
#include <chrono>


#include "representation.hpp"
#include "tabusearch.hpp"

void print_usage()
{
    std::cout << "Uso\n";
    std::cout << "bep -i <ruta/de/instancia> -o <ruta/de/log> [-n <iteraciones>] [-l <largo_lista_tabu>] [-r <reset>] [-v <nivel>]\n\n";
    std::cout << "Donde:\n";
    std::cout << "\truta/de/instancia: Es la ruta al archivo de la instancia que se va a probar." << '\n';
    std::cout << "\truta/de/log: Es la ruta del archivo output para la instancia." << '\n';
    std::cout << "\titeraciones (default=100): Es la cantidad máxima de iteraciones para el algoritmo." << '\n';
    std::cout << "\tlargo_lista_tabu (default=6): Es el largo de la lista tabu." << '\n';
    std::cout << "\treset(default=500): Cantidad de resets del algoritmo." << '\n';
    std::cout << "\tnivel(default=0): Mas alto, mas output" << '\n';
}


int main( int argc, char* argv[] )
{
    srand (time(0));

    // Lectura de parametros
    if (argc < 5 || 13 < argc) {
        print_usage();
        return 1;
    }
    std::string input_file = "";
    std::string output_file = "";
    int max_iterations = 100;
    int tabu_list_len = 6 ;
    int loglvl = 0;
    int resets = 1;
    
    int c ;
    while((c = getopt(argc, argv, "i:o:n:l:v:r:")) != -1) 
    {
        switch(c)
        {
            case 'i':
                if (optarg) input_file = optarg;
                break;
            case 'o':
                if (optarg) output_file = optarg;
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
            case 'r':
                if(optarg) resets = std::atoi(optarg);
                break;
        }
    }
    
    if (loglvl >= 1) {
        std::cout << "Parametros del programa\n";
        std::cout << "Input file: " << input_file << "\n";
        std::cout << "Max iterations: " << max_iterations << '\n';
        std::cout << "Tabu list length: " << tabu_list_len << '\n';
        std::cout << "logging level: " << loglvl << "\n";
        std::cout << "Resets: " << resets << "\n\n";

        std::cout << "Cargando Scenario \n";
    }    

    // Cargar el escenario de la instancia
    Scenario scenario (input_file, loglvl);
    if (loglvl >= 2) {
        scenario.print();
    }

    // Creamos un nuevo Tabu Search con los parametros dados
    TabuSearch solver (max_iterations, tabu_list_len, loglvl);
    if(loglvl == 2) {
        std::cout << "\nTabu Search:";
        std::cout << "\n";
        solver.print();
        std::cout << "\n";
    }

    Solution best(0);
    auto start = std::chrono::system_clock::now();
    while(resets-- > 0) {
        if (loglvl >= 1) {
            std::cout << "Resets restantes" << resets << "\n";
            std::cout << "Buscando solucion inicial\n";
        }
        // Buscamos una solucion inicial para entregarsela a Tabu Search.
        Solution initial = scenario.get_initial_solution();
        
        if (loglvl >= 1) {
            if (loglvl == 2) {
                std::cout << "\nSolucion inicial\n";
                initial.print();
                std::cout << "\n";
            }
            std::cout << "Comenzando TabuSearch\n";
        }

        // Hacemos correr el algoritmo con el escenario y solucion inicial.
        Solution iteration_best = solver.run(scenario, initial);

        //Guardamos la nueva solucion encontrada si tiene menor score (menor tiempo)
        if (best.get_score() == 0 || iteration_best.get_score() < best.get_score()) {
            best = iteration_best;
        }
    }


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> 
                (std::chrono::system_clock::now() - start);
    // Output
    if (loglvl >= 1) {
        std::cout << "\nTerminado\n";
        if (loglvl == 2) {
            std::cout << "Mejor solucion\n";
            best.print();
        }
    }    
    std::cout << "Funcion objetivo:\t" << best.get_score() << "\n";
    std::cout << "Iteraciones:\t" << solver.get_iterations() << "\n";
    std::cout << "Largo Lista Tabu:\t" << solver.get_tabulist_size() << "\n";
    if (loglvl >= 0) {
        std::cout << "Tiempo de ejecucion:\t" << (float)duration.count()/1000 << "[s]\n";
    }

    // A archivo
    std::ofstream file;
    file.open (output_file);
    file << "Funcion objetivo:\t" << best.get_score() << "\n";
    file << "Iteraciones:\t" << solver.get_iterations() << "\n";
    file << "Largo Lista Tabu:\t" << solver.get_tabulist_size() << "\n";
    if (loglvl >= 0) {
        std::cout << "Tiempo de ejecucion:\t" << (float)duration.count()/1000 << "[s]\n";
    }
    
    scenario.write(file, best);
    file.close();


    
    
    return 0;
}