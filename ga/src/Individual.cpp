/*
 * Individual.cpp
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#include "Individual.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <assert.h>

Individual::Individual()
{
	chromLength = 0;
	fitness = 0;
	distance = 0;
}
Individual::Individual(int len) {
	// TODO Auto-generated constructor stub
	chromLength = len;
	fitness = -1;
	distance = 0;
}

Individual::~Individual() {
	// TODO Auto-generated destructor stub
}

static bool cluster_sorter(int a, int b)
{
	return customers[a-1].depot_id < customers[b-1].depot_id;
}

void Individual::Init(){

	for(int i=0; i<chromLength; i++)
	{
		chromosome[i] = customers[i].id;
	}
	std::sort(chromosome, chromosome+chromLength, cluster_sorter);

	/*
	for(int i = 0; i < chromLength; i++){
		chromosome[i] = i+1;
	}

	// Fisher�Yates shuffle
	for (int i = 0; i < chromLength; i++)
	{
		int j = IntInRange(i, chromLength);
		std::swap(chromosome[i], chromosome[j]);
	}*/

}

void Individual::Mutate(double pm){
	if (Flip(pm))
	{
		Inversion_mutation();
	}
}

void Individual::Inversion_mutation()
{
		int a = IntInRange(0, chromLength);
		int b = IntInRange(0, chromLength);
		if(a>b)
			std::swap(a,b);
		
		std::stack<int> inversed;
		for(int i=a; i<=b; i++)
		{
			inversed.push(chromosome[i]);
		}

		for(int i=a; i<=b; i++)
		{
			chromosome[i] = inversed.top();
			inversed.pop();
		}
}

void Individual::Swap_mutation()
{
		int a = IntInRange(0, chromLength);
		int b = IntInRange(0, chromLength);

		std::swap(chromosome[a], chromosome[b]);
}

void Individual::copy_into(Individual* individual)
{

	individual->chromLength = chromLength;
	individual->fitness = fitness;
	individual->distance = distance;
	std::memcpy(individual->chromosome, chromosome, sizeof(int) * chromLength);
}

void Individual::print_string()
{
	for (int i = 0; i < chromLength; i++)
		std::cout << chromosome[i]<<",";
	std::cout << std::endl;
}