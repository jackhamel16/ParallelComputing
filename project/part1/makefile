all: omp fftw fftw_threads

omp: omp_example.cpp
	g++ -fopenmp -o omp omp_example.cpp

fftw: fftw_example.cpp
	g++ -lfftw3 -lm -o fftw fftw_example.cpp

fftw_threads: fftw_threads_example.cpp
	g++ -lfftw3_omp -lfftw3 -lm -fopenmp -o fftw_threads fftw_threads_example.cpp

fftw_test:
	time ./fftw
	time ./fftw_threads

clean:
	rm -f omp fftw fftw_threads
