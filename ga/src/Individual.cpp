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
#include "Evaluate.h"
#include <queue>
#include <exception>

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


struct Saving
{
	float saving;
	int i;
	int j;
};

struct CompareSaving
{
	bool operator() (const Saving& lhs, const Saving& rhs)
	{
		return lhs.saving<rhs.saving;
	}
};



void Individual::Init(){

	for(int i=0; i<chromLength; i++)
	{
		chromosome[i] = customers[i].id;
	}

	std::sort(chromosome, chromosome+chromLength, cluster_sorter);
	int d_id = customers[chromosome[0]-1].depot_id;
	int s = 0;
	int _i = 0;
	for(; _i<chromLength; _i++)
	{
		auto _id = chromosome[_i];
		if(d_id != customers[_id-1].depot_id)
		{
			depot_ranges.push_back( std::make_tuple(d_id, s, _i-1));
			s = _i;
			d_id = customers[_id-1].depot_id;
		}
	}
	depot_ranges.push_back( std::make_tuple(d_id, s, _i-1));
	
	// saving
	for(auto d:depot_ranges)
	{

		auto _s = std::get<1>(d);
		auto _e = std::get<2>(d);
		Depot _d = depots[std::get<0>(d) - 1];

		auto _range = _e - _s + 1;
		float* saving_matrix = new float[_range*_range];
		std::priority_queue<Saving, std::vector<Saving>, CompareSaving> saving_list;

		for(int i=_s; i<=_e; i++)
		{
			Customer c_i = customers[ chromosome[i] - 1];
			for(int j=i+1; j<=_e; j++)
			{
				Customer c_j = customers[ chromosome[j] - 1];
				//std::cout<<i<<","<<j<<","<<_range<<std::endl;
				auto saving_value = eucledian_distance(_d.x, _d.y, c_i.x, c_i.y) + eucledian_distance(_d.x, _d.y, c_j.x, c_j.y) - eucledian_distance(c_i.x, c_i.y, c_j.x, c_j.y);
				//std::cout<<" got saving_value "<<saving_value<< std::endl;
				saving_matrix[ ( (i-_s)*_range) + (j-_s)] =saving_value;
				Saving _s{saving_value, i, j};
				saving_list.push(_s);
			}
		}

		std::cout<<"--------"<<_d.id<<"------------"<<std::endl;

		while(!saving_list.empty())
		{
			std::cout<<"val: ("<<saving_list.top().i<<","<<saving_list.top().j<<") : "<< saving_list.top().saving<<std::endl;
			saving_list.pop();
		}

		delete[] saving_matrix;
	}

	assert(false);




	// NN scheduling
	/*std::vector<int> routes;
	Calculate_route_len(this, false, &routes);

	s = 0;
	for(auto r : routes)
	{
		std::vector<int> others;
		int lc_ind = IntInRange(s,r+1);
		auto lc = customers[chromosome[lc_ind] - 1];
		std::swap(chromosome[s], chromosome[lc_ind]);


		for(int _n=s+1; _n<=r; _n++ )
		{
			float _min = INT_MAX;
			int _min_i = _n;
			for(int i= _n+1; i<=r; i++)
			{
				auto nc = customers[chromosome[i] -1];
				float _dist = eucledian_distance( lc.x, lc.y, nc.x, nc.y );
				if(_dist < _min)
				{
					_min = _dist;
					_min_i = i;
					lc = nc;
				}
			}
			std::swap(chromosome[_n], chromosome[_min_i]);

		}

	}

*/


	
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