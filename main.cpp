#include <iostream>
#include <unistd.h>
#include <cstdlib>

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
    const char* intput_file = ""; 
    const char* output_file = "";
    int max_iterations = 100;
    int tabu_list_len = 6 ;
    int loglvl = 0;
    
    int c ;
    while((c = getopt(argc, argv, "i:o:n:l:v:")) != -1) 
    {
        switch(c)
        {
            case 'i':
                if (optarg) intput_file = optarg;
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
        std::cout << intput_file << "\n";
        std::cout << output_file << '\n';
        std::cout << max_iterations << '\n';
        std::cout << tabu_list_len << '\n';
        std::cout << loglvl << '\n';
    }    



}