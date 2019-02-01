# This script will run all three parts of homework 2 for cmse 401
# It will run for various matrix sizes and save timiings in txt file
# then will run a python script to parse timings and generate a plot
# and table
echo
echo Compiling...

g++ -o part1 part1.cpp
g++ -o part2 part2.cpp
g++ -o part3 part3.cpp

sizes=(200 1000 5000 8000 10000 15000 20000 40000)

> timings.dat

echo
echo Running part 1...

for size in "${sizes[@]}"
do
  echo Matrix size = $size
  ./part1 $size >> timings.dat
  ./part1 $size >> timings.dat
  ./part1 $size >> timings.dat
done

echo
echo Running part 2...

for size in "${sizes[@]}"
do
  echo Matrix size = $size
  ./part2 $size >> timings.dat
  ./part2 $size >> timings.dat
  ./part2 $size >> timings.dat
done

echo
echo Running part 3...

for size in "${sizes[@]}"
do
  echo Matrix size = $size
  ./part3 $size >> timings.dat
  ./part3 $size >> timings.dat
  ./part3 $size >> timings.dat
done

python timings.py
