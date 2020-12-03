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


struct EvaluationResult
{
	float fitness;
	float distance;
};


EvaluationResult Eval(Individual *individual);
float Calculate_route_len(Individual* individual, bool draw_in_js);

extern std::vector<Customer> customers;
extern std::vector<Depot> depots;

#endif /* EVALUATE_H_ */
