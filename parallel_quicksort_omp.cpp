/*! 
 *	\brief	Parallel quicksort using OpenMP
 *
 */
#include <omp.h>
#include <iostream>
#include <algorithm>
#include <vector>

//! global variables
const std::size_t g_maxSize = 10000000;	//!< Maximum array size
const std::size_t g_maxThreads = 128;		//!< Maximum number of threads
std::size_t g_numThreads;
std::size_t g_size;

int main(int argc, const char *argv [])
{
	double start_time, end_time;

	// Read command line args if any
	g_size = (argc > 1)? atoi(argv[1]) : g_maxSize;
	g_numThreads = (argc > 2)? atoi(argv[2]) : g_maxThreads;
	if (g_size > g_maxSize) g_size = g_maxSize;
	if (g_numThreads > g_maxThreads) g_numThreads = g_maxThreads;
	
	// Initilize array data
	std::vector<int> arrayData(g_size);
	std::generate(arrayData.begin(), arrayData.end(), [](){
		return rand() % 100;
	});
	
	// Set the number of threads available to open mp
	omp_set_num_threads(g_numThreads);		
	
	start_time = omp_get_wtime();
	
	// DO WORK	
	#pragma omp parallel shared(arrayData)
	{
		#pragma omp single nowait
		{
			std::sort(arrayData.begin(), arrayData.end());
		}
	}
	
	end_time = omp_get_wtime();

	bool sorted = std::is_sorted(arrayData.begin(), arrayData.end());
	std::cout << "It took " << end_time - start_time << " seconds" << std::endl;
	
	if(sorted)
		std::cout << "Array is sorted" << std::endl;
	else
		std::cout << "Array is not sorted" << std::endl;
	return 0;
}
