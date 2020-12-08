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
#include <algorithm>

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
	int customer_i;
	int customer_j;
};

struct CompareSaving
{
	bool operator() (const Saving& lhs, const Saving& rhs)
	{
		return lhs.saving<rhs.saving;
	}
};


int in_route(int cust_id, const std::vector<std::vector<int>>& _route)
{
	for(int i=0; i<_route.size(); i++)
	{
		auto r = _route[i];
		for(auto c:r)
		{
			if(cust_id == c)
				return i;
		}
	}
	return -1;
}

double calculate_route_demand(const std::vector<int> _route)
{
	auto _demand = 0;
	for(auto r:_route)
	{
		_demand += customers[r-1].demand;
	}
	return _demand;
}

void Individual::SavingMethod(std::vector<std::tuple<int, int, int >>& depot_ranges)
{

	int chrom_ind = 0;
	std::priority_queue<int> pq;

	for(auto d_r:depot_ranges)
	{

		Depot _d = depots[std::get<0>(d_r) - 1];
		auto _s = std::get<1>(d_r);
		auto _e = std::get<2>(d_r);

		auto _range = _e - _s + 1;
		float* saving_matrix = new float[_range*_range];
		std::priority_queue<Saving, std::vector<Saving>, CompareSaving> saving_list;

		std::vector<int> cids;

		for(int i=_s; i<_e; i++)
		{
			Customer c_i = customers[ chromosome[i] - 1];
			cids.push_back(c_i.id);
			for(int j=i+1; j<=_e; j++)
			{
				Customer c_j = customers[ chromosome[j] - 1];
				//std::cout<<i<<","<<j<<","<<_range<<std::endl;
				auto saving_value = eucledian_distance(_d.x, _d.y, c_i.x, c_i.y) + eucledian_distance(_d.x, _d.y, c_j.x, c_j.y) - eucledian_distance(c_i.x, c_i.y, c_j.x, c_j.y);
				//std::cout<<" got saving_value "<<saving_value<< std::endl;
				saving_matrix[ ( (i-_s)*_range) + (j-_s)] = saving_value;
				Saving _s{saving_value, c_i.id, c_j.id};
				saving_list.push(_s);
			}
		}

		std::cout<<"--------"<<_d.id<<"------------"<<std::endl;
		
		std::vector<std::vector<int>> routes;
		while(!saving_list.empty())
		{
			auto _s = saving_list.top();
			saving_list.pop();

			int r_i = (in_route( _s.customer_i, routes ));
			int r_j = (in_route( _s.customer_j, routes ));
			//std::cout<<r_i<<"---"<<r_j<<std::endl;
			
			if(r_i == -1 && r_j == -1)
			{
				double _demand = customers[_s.customer_i-1].demand + customers[_s.customer_j-1].demand;
				if(_demand <= _d.capacity)
				{
					std::vector<int> _route{_s.customer_i, _s.customer_j};
					routes.push_back(_route);

					cids.erase(std::remove(cids.begin(), cids.end(), _s.customer_i), cids.end());
					cids.erase(std::remove(cids.begin(), cids.end(), _s.customer_j), cids.end());

				}
			}
			else if(r_i != -1 && r_j != -1)
			{
				// try to merge
				if(r_i != r_j)
				{
					auto r_i_v = routes[r_i];
					auto _demand_r_i = calculate_route_demand(r_i_v);
					auto r_j_v = routes[r_j];
					auto _demand_r_j = calculate_route_demand(r_j_v);

					if(_demand_r_i+_demand_r_j <= _d.capacity)
					{
						if(r_i_v[ r_i_v.size()-1] == _s.customer_i && r_j_v[0] == _s.customer_j)
						{
							for(auto _c:r_j_v)
								routes[r_i].push_back(_c);
							routes.erase(routes.begin()+r_j);
							
						}
						else if(r_j_v[ r_j_v.size()-1] == _s.customer_j && r_i_v[0] == _s.customer_i)
						{
							for(auto _c:r_i_v)
								routes[r_j].push_back(_c);
							routes.erase(routes.begin()+r_i);

						}
					}
				}
			}
			else if(r_i == -1 || r_j == -1)
			{
				if(r_i == -1)
				{
					auto r_j_v = routes[r_j];
					auto _demand_r_j = calculate_route_demand(r_j_v);

					if(_demand_r_j + customers[_s.customer_i-1].demand <= _d.capacity)
					{
						if(routes[r_j][0]==_s.customer_j)
						{
							routes[r_j].insert( routes[r_j].begin(), _s.customer_i);
							cids.erase(std::remove(cids.begin(), cids.end(), _s.customer_i), cids.end());

						}
						else if(routes[r_j][routes[r_j].size()-1]==_s.customer_j)
						{
							routes[r_j].push_back(_s.customer_i);
							cids.erase(std::remove(cids.begin(), cids.end(), _s.customer_i), cids.end());

						}
					}
				}
				else if(r_j == -1)
				{
					auto r_i_v = routes[r_i];
					auto _demand_r_i = calculate_route_demand(r_i_v);

					if(_demand_r_i + customers[_s.customer_j-1].demand <= _d.capacity)
					{
						if(routes[r_i][routes[r_i].size()-1]==_s.customer_i)
						{
							routes[r_i].push_back(_s.customer_j);
							cids.erase(std::remove(cids.begin(), cids.end(), _s.customer_j), cids.end());

						}
						else if(routes[r_i][0]==_s.customer_i)
						{
							routes[r_i].insert( routes[r_i].begin(), _s.customer_j);
							cids.erase(std::remove(cids.begin(), cids.end(), _s.customer_j), cids.end());
						}
					}
				}
			}
		}

		for(auto c:cids)
		{
			routes.push_back( std::vector<int>{c});
		}

		for(auto r:routes)
		{
			double _demand = 0;
			for(auto c:r)
			{
				_demand+= customers[c-1].demand;
				chromosome[chrom_ind] = c;
				chrom_ind++;
				pq.push(c);
				std::cout<<c<<"-";
			}
			std::cout<<" : "<<_demand<<std::endl;
		}


		
		delete[] saving_matrix;
	}

	
		
		while(!pq.empty())
		{
			std::cout<<pq.top()<<"-";//<<std::endl;
			pq.pop();
		}
		std::cout<<std::endl;
}


void Individual::Init(Individual* other)
{
	for(int i=0; i<chromLength; i++)
	{
		chromosome[i] = other->chromosome[i];
	}
}

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
	
	SavingMethod(depot_ranges);
	for(int i=0; i<chromLength; i++)
	{
		std::cout<<chromosome[i]<<"-";
	}
	std::cout<<std::endl;
	// saving
	
	//assert(false);




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