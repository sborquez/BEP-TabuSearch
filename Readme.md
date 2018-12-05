## Uso

```bash
bep -i <ruta/de/instancia> -o <ruta/de/log> [-n <iteraciones>] [-l <largo_lista_tabu>] [-v <nivel>]
```

Donde 

* __ruta/de/instancia__: Es la ruta al archivo de la instancia que se va a probar.
* __ruta/de/log__: Es la ruta del archivo output para la instancia.
* __iteraciones__ (default=100): Es la cantidad máxima de iteraciones para el algoritmo.  
* __largo_lista_tabu__ (default=6): Es el largo de la lista tabú.
* __nivel__ (default=0): Es el nivel de output por consola. Donde 0 es el normal, 1 incluye información por iteración y 2 incluye información extra por cada paso.
