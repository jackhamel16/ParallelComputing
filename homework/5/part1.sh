#!/bin/bash

nxy=(300 400 500 600 700)
nt=(6000 8000 10000 12000 14000)

#for i in "${nxy[@]}"
#do
#  echo $i
#done

echo "Spatial size runs:"
for n in "${nxy[@]}"
do
  for i in {1..3}
  do
    echo "nxy = $n, run = $i"
    time ./serial $n >> /dev/null
  done
done

#for n in "${nt[@]}"
#do
#  for i in {1..3}
#  do
#    echo "nt = $n, run = $i"
#    time ./serial 500 $n >> /dev/null 
#  done
#done

