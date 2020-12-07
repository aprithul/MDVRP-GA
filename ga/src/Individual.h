/*
 * Individual.h
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include "Const.h"
#include <vector>
#include <cstring>
#include <stack>
#include <tuple>

struct Depot
{
	int id;
	double x;
	double y;
	double capacity;
	int customer_count;
};


struct Customer
{
	int id;
	int depot_id;
	double x;
	double y;
	double demand;

};

extern std::vector<Customer> customers;
extern std::vector<Depot> depots;

class Individual {
public:
	Individual();
	Individual(int chromLen);
	virtual ~Individual();

	int chromLength;
	std::vector<std::tuple<int, int, int >> depot_ranges;
	int chromosome[MAX_CHROMLENGTH];
	double fitness;
	double distance;
	
	void Init();
	void Mutate(double pm);
	void Inversion_mutation();
	void Swap_mutation();
	void copy_into(Individual* individual);
	void print_string();
};

#endif /* INDIVIDUAL_H_ */
