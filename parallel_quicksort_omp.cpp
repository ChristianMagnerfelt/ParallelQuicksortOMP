/*! 
 *	\brief	Parallel quicksort using OpenMP
 *	\author	Christian Magnerfelt
 *	\email	magnerf@kth.se	
 */
#include <omp.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

// global variables
const long g_sortCutoff = 1000;				//!< cutoff to use std::sort
const std::size_t g_numOfPivots = 10;		//!< number of pivots for parition. only one is choosen in the end.
const std::size_t g_maxSize = 100000000;	//!< Maximum array size
const std::size_t g_maxThreads = 128;		//!< Maximum number of threads
std::size_t g_numThreads;
std::size_t g_size;

/*!		\brief			Templated parallel quicksort
 *		\description	Sorts range in parallel using quicksort given that iterators are random-access
 * 						and value supports assigment and operator<
 */
template <typename T, typename RandomIt>
void parallelQuicksort(RandomIt start, RandomIt end)
{
	// Create a staic array of pivots
	static std::vector<T> pivots (g_numOfPivots);
	
	// Calculate the number of elements in the range
	auto distance = std::distance(start, end);
	// If number of elements are less than cutoff then use std::sort
	if(distance < g_sortCutoff)
	{
		std::sort(start, end);
	}
	else
	{
		// Select pivots on random
		RandomIt it;
		for(std::size_t i = 0; i < pivots.size(); ++i)
		{
			it = start;
			std::advance(it, rand() % distance);
			pivots[i] = *it;
		}
		// Sort pivots in acending order
		std::sort(pivots.begin(), pivots.end());
		
		// Select the pivot value from the middle of the pivot vector
		T pivotValue = pivots[pivots.size()/2];
		
		// Partition values so those less than pivot value are placed in front of the array
		RandomIt middle = std::partition(start, end, [&pivotValue](const T & value){
			if(value < pivotValue)
				return true;
			else
				return false;
		});
		
		// Sort as normal if middle is equal to start or end. 
		// All elements are either the same or we selected a max or min value as pivot
		if(middle == start || middle == end)
		{
			std::sort(start, end);
			return;
		}
		
		// Do parallel quicksort on each partition
		#pragma omp task shared(start, middle)
		{
			parallelQuicksort<T, RandomIt>(start, middle);
		}
		#pragma omp task shared(middle, end)
		{
			parallelQuicksort<T, RandomIt>(middle, end);
		}
		// Wait for tasks to complete
		#pragma omp taskwait
	}
}
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
	
	// Do parallel quicksort on array data
	#pragma omp parallel shared(arrayData)
	{
		#pragma omp single nowait
		{
			parallelQuicksort<int, std::vector<int>::iterator>(arrayData.begin(), arrayData.end());
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
