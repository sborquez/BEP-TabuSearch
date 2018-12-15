#!/bin/bash

if [ "$#" -lt 2 ]; then
  echo "Usage: $0  /ruta/a/carpeta/de/instancias largo_lista_tabu_1 largo_lista_tabu_2 ..."
  exit 1
fi
instances_path=$1

make bep
for file in $instances_path/*.txt
do
    ./run.sh $file ${@:2}
done