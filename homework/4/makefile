all: original fast omp 

original: main_process_orig.c
	gcc -c main_process_orig.c
	gcc -l lpng16 -c png_util.c
	gcc -o process_orig -lm -l png16 main_process_orig.o png_util.o

fast: main_process.c
	gcc -c -O3 main_process.c
	gcc -l lpng16 -c -O3 png_util.c
	gcc -O3 -o process_fast -lm -l png16 main_process.o png_util.o

omp: main_process_omp.c
	gcc -fopenmp -c -O3 main_process_omp.c
	gcc -l lpng16 -c -O3 png_util.c
	gcc -O3 -fopenmp -o process_omp -lm -l png16 main_process_omp.o png_util.o

test: process 
	./process cube.png test.png

clean:
	rm *.o
	rm process 
