debug: parallel_quicksort_omp.cpp
	g++ -Wall -g -O0 -o ParallelQSOMP parallel_quicksort_omp.cpp -fopenmp -std=c++0x
	
release: parallel_quicksort_omp.cpp
	g++ -Wall -O3 -o ParallelQSOMP parallel_quicksort_omp.cpp -fopenmp -std=c++0x
	
clean:
	rm MatrixSumOMP
