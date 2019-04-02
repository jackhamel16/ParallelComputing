all: slow serial omp threads parallel

slow: matvec_slow.cpp
	g++ -o slow matvec_slow.cpp matvec.h

serial: matvec_serial.cpp
	g++ -lfftw3 -lm -o serial matvec_serial.cpp matvec.h

omp: matvec_omp.cpp
	g++ -lfftw3 -lm -fopenmp -o omp matvec_omp.cpp matvec.h 

threads: matvec_threads.cpp
	g++ -lfftw3_omp -lfftw3 -lm -fopenmp -o threads matvec_threads.cpp matvec.h 

parallel: matvec_parallel.cpp
	g++ -lfftw3_omp -lfftw3 -lm -fopenmp -o parallel matvec_parallel.cpp matvec.h 

clean:
	rm serial 
