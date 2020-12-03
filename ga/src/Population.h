/*
 * Population.h
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "Const.h"
#include "Options.h"
#include "Individual.h"

class Population {
public:
	Population(Options options);
	virtual ~Population();
	//------------------------
	Options options;
	Individual* offsprings[5];// = new Individual*[5];
	Individual *members[MAXPOP];
	double rank_probability[MAXPOP];
	static double avg, avgDist, min, minDist, max, sumFitness;
	static Individual* best_solution;

	void Init();
	void Evaluate();
	void Generation(Population *child);
	void Report(unsigned long int gen);
	void Report_file();

	void Statistics();

	int LinearRankSelector();
	int ProportionalSelector();
	int NaryTournament(int n);
	int GetBest();
	void XoverAndMutate(Individual *p1, Individual *p2, Individual *c1, Individual *c2);
	void TwoPoint(Individual *p1, Individual *p2, Individual *c1, Individual *c2);
	void OnePoint(Individual *p1, Individual *p2, Individual *c1, Individual *c2);
	void OrderCrossover(int s, int e, Individual* p1, Individual* p2, Individual* c1);
	void Test();
	
	void IteratedSwap(Individual* individual);
};

#endif /* POPULATION_H_ */
