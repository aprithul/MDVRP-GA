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
	Individual(int chromLen);
	virtual ~Individual();

	int chromLength;
	int chromosome[MAX_CHROMLENGTH];
	double fitness;

	void Init();
	void Mutate(double pm);

	void print_string();
};

#endif /* INDIVIDUAL_H_ */
