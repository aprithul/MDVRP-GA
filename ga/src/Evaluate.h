/*
 * Evaluate.h
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include "Individual.h"
#include <vector>

double Eval(Individual *individual);

extern std::vector<Customer> customers;
extern std::vector<Depot> depots;

#endif /* EVALUATE_H_ */
