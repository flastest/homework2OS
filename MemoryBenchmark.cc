#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <cassert>


using data_t = uint32_t;

/*
 * um homework 2, i didn't just copy and paste this from systems..........
 *  I ACTUALLY CHANGED IT SO LIEK I KNOW WHAT IT DOES NOW
 *
 */


// creates a random array of bytes given a size
std::vector<data_t> random_array(int size)
{
	std::mt19937 mt_rand(time(0));
	std::vector<data_t> byte_vector(size);

	for (int i = 0; i < size; i++)
	{
		byte_vector[i] = (mt_rand()%255);
	}

	return byte_vector;
}





/*
 * 
 * 
 * 
 */
float page_thing_that_Im_measuring(data_t page_size, data_t num_pages)
{
	
  //buffer size is number of ints, but I want number of bytes
	
	std::vector<data_t> bytes = random_array(page_size*num_pages*sizeof(int));


	//determine how many iters to do
//	int iters = buffer_size < 100000 ? buffer_size * 2048 : buffer_size;
	int iters = 10000000;


	std::vector<int> indexes;

	//make random permutation of indexes
	for (int i = 0; i < iters; i ++)
	{
		indexes.push_back((i)%(page_size));
	}
	random_shuffle(indexes.begin(),indexes.end());


	int jump = page_size / sizeof(int);

	//time "iters" amount of reads from bytes vector 
	auto startTime = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < num_pages * jump; i += jump)
	{
		indexes[i] += 1;
	}


	
	auto finTime = std::chrono::high_resolution_clock::now();
	

	float t = std::chrono::duration_cast<std::chrono::nanoseconds>( finTime - startTime ).count();
	
	return t/iters;
	
}








// takes an input of some number of trials and a buffer size and runs
// that number of trials with the given buffer size and returns the smallest
// time it took to do the thing. This returns the minimum
float get_min_from_n_trials(int num_trials, int num_pages)
{
	
	float min = page_thing_that_Im_measuring(4,num_pages);


	for (int i = 0; i < num_trials; i++)
	{
		float new_time = page_thing_that_Im_measuring(4,num_pages);
		min = new_time < min ? new_time : min;
	}

	return min;


}







int main()
{

	uint64_t smallest_trial = pow(2,9);
	uint64_t biggest_trial = pow(2,40);
	int num_trials = 16;

	std::cout<<"# Bytes\ttime"<<std::endl;

	for (uint64_t i = smallest_trial; i <= biggest_trial; i *= 2)
	{
		float time = get_min_from_n_trials(num_trials,i);
		std::cout<<i*sizeof(data_t)<<"\t"<<time<<std::endl;

	}

	return 0;
}

