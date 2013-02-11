Parallel quicksort
-----------------
Author:
Christian Magnerfelt

Email:
magnerf@kth.se

Description:
A program that sorts test data in form of arrays using a parallel quicksort algorithm.
Parallism is implemented by open mp using tasks. Each task is represented by the partitions created by the quicksort algorithm.
The parallel quicksort algorithm is implemented to support any container that support Random-Access iterators and value type which support assigment and operator<
It also possible to specify the quicksort cutoff for using std::sort as well as the number of randomly selected pivots.

Compiler:
This program requires at least GCC 4.4.7

Usage:
	./ParallelQSOMP {array size} {num of threads}
	
Build:
	make debug
	make release

