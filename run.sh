#!/bin/bash

if [ "$#" -lt 2 ]; then
  echo "Usage: $0  /ruta/a/intancia largo_lista_tabu_1 largo_lista_tabu_2 ..." 
  exit 1
fi
loglvl=0
instance_path=$1
resets=1000
base_name=$(basename $instance_path .txt)
make bep
for len in ${@:2}
do
    output_path=$(date -d "today" +"%H%M%S")
    output_path="$base_name.$len.$output_path.log"
    mydir=$(mktemp "XXX")
    echo $output_path
    if [ $loglvl -gt 0 ]; then
      { time ./bep -i $instance_path -o $output_path -n 800 -l $len -v $loglvl -r $resets; } 2> $mydir > "$output_path.output"
    else
      { time ./bep -i $instance_path -o $output_path -n 800 -l $len -v $loglvl -r $resets; } 2> $mydir
    fi
    tail -n 2 $mydir
    tail -n 2 $mydir >> $output_path
    rm $mydir
    sleep 1
done