#!/bin/bash

nxy=(300 400 500 600 700)

echo "Spatial size runs:"
for n in "${nxy[@]}"
do
  for i in {1..3}
  do
    echo "nxy = $n, run = $i"
    ./cuda $n
    echo
  done
done
