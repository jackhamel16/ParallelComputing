echo Compiling...

g++ -o slow part1_slow.cpp
g++ -o fast part1.cpp

echo
echo Running Slow Code...

time ./slow

echo
echo Running Fast Code...

time ./fast

