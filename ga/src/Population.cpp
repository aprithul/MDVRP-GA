/*
 * Population.cpp
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#include "Population.h"
#include <assert.h>
#include "Evaluate.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

#ifndef WIN32
#include <emscripten.h>
#endif

Population::Population(Options opts) {
	options = opts;
	avg = min = max = sumFitness = -1;
	assert(options.popSize <= MAXPOP);
	for (int i = 0; i < options.popSize; i++){
		members[i] = new Individual(options.chromLength);
		members[i]->Init();
	}
}

Population::~Population() {
	// TODO Auto-generated destructor stub
}

void Population::Init(){
	Evaluate();
}

void Population::Evaluate(){
	for (int i = 0; i < options.popSize; i++){
		members[i]->fitness = Eval(members[i]);
	}
}

void Population::Statistics(){
	sumFitness = 0;
	min = members[0]->fitness;
	max = members[0]->fitness;
	for(int i = 0; i < options.popSize; i++){
		sumFitness += members[i]->fitness;
		if(min > members[i]->fitness)
			min = members[i]->fitness;
		if(max < members[i]->fitness)
			max = members[i]->fitness;
	}
	avg = sumFitness/options.popSize;
}

#ifndef WIN32
EM_JS(void, call_js_update_chart, (double avg), {
	update_chart(avg);
});
#endif

void Population::Report(unsigned long int gen){
	char printbuf[1024];
	sprintf(printbuf, "%4i \t %f \t %f \t %f\n ", (int)gen, min, avg, max);
	WriteBufToFile(std::string(printbuf), options.outfile);
	std::cout << printbuf;
#ifndef WIN32
	call_js_update_chart(avg);
#endif
}

static bool comp(Individual* a, Individual* b)
{
	return (a->fitness < b->fitness);
}
float thirds[3] = {};

void Population::Generation(Population *child){
	int pi1, pi2, ci1, ci2;
	Individual *p1, *p2, *c1, *c2;
	//std::sort(members, members + options.popSize, comp);
	for(int i = 0; i < options.popSize; i += 2){
		//pi1 = ProportionalSelector();
		//pi2 = ProportionalSelector();
		//pi1 = LinearRankSelector();
		//pi2 = LinearRankSelector();
		//pi1 = GetBest(); //NaryTournament(2);
		//pi2 = GetBest(); //NaryTournament(2);
		pi1 = NaryTournament(2);
		pi2 = NaryTournament(2);
		ci1 = i;
		ci2 = i + 1;

		p1 = members[pi1]; p2 = members[pi2];
		c1 = child->members[ci1]; c2 = child->members[ci2];

		XoverAndMutate(p1, p2, c1, c2);
	}
	//std::cout << x << "---" << y << std::endl;
}

int Population::LinearRankSelector()
{
	int cumulitive_prob = (options.popSize + 1) * (options.popSize / 2);

	int prob = IntInRange(1, cumulitive_prob+1);
	//std::cout << "prob: " << prob << std::endl;;

	int selection = 0;
	while (prob > 0)
	{
		selection++;
		prob = prob - selection;
	}

	return selection-1;

}


int Population::GetBest()
{
	int _max = -1;
	int selection = 0;
	for(int i=0; i<options.popSize; i++)
	{
		if(members[i]->fitness > _max)
		{
			_max = members[i]->fitness;
			selection = i;
		}
	}

	return selection;
}

int Population::NaryTournament(int n)
{

	int* indices = new int[n];

	for (int i = 0; i < n; i++)
	{
		indices[i] = IntInRange(0, options.popSize);
	}

	int selection = 0;
	int _max = -1;
	for (int i=0; i<n; i++)
	{
		int index = indices[i];
		if (members[index]->fitness > _max)
		{
			_max = members[index]->fitness;
			selection = index;
		}
	}

	return selection;
}


int Population::ProportionalSelector(){
	int i = -1;
	float sum = 0;
	float limit = RandomFraction() * sumFitness;
	do {
		i = i + 1;
		sum += members[i]->fitness;
	} while (sum < limit && i < options.popSize-1 );

	return i;
}

void Population::XoverAndMutate(Individual *p1, Individual *p2, Individual *c1, Individual *c2){

	for(int i = 0; i < options.chromLength; i++){ //First copy
		//assert(c1 != nullptr && c2 != nullptr);
		//assert(c1->chromosome != nullptr && c2->chromosome != nullptr);
		//std::cout<<c1->chromLength<<","<<c2->chromLength<<std::endl;
		c1->chromosome[i] = p1->chromosome[i];
		c2->chromosome[i] = p2->chromosome[i];
	}

	if(Flip(options.px)){ // if prob, then cross/exchange bits
		//OnePoint(p1, p2, c1, c2);
		//TwoPoint(p1, p2, c1, c2);
		int s = IntInRange(0, options.chromLength);
		int e = IntInRange(0, options.chromLength);
		if (s > e)
			std::swap(s, e);

		OrderCrossover(s, e, p1, p2, c1);
		OrderCrossover(s, e, p2, p1, c2);
	}

	c1->Mutate(options.pm);
	c2->Mutate(options.pm);
}

void Population::OnePoint(Individual *p1, Individual *p2, Individual *c1, Individual *c2){ //not debugged
	int t1 = IntInRange(0, options.chromLength);
	for(int i = t1; i < options.chromLength; i++){
		c1->chromosome[i] = p2->chromosome[i];
		c2->chromosome[i] = p1->chromosome[i];
	}
}

void Population::TwoPoint(Individual *p1, Individual *p2, Individual *c1, Individual *c2){ //not debugged
	int t1 = IntInRange(0, options.chromLength);
	int t2 = IntInRange(0, options.chromLength);
	int xp1 = std::min(t1, t2);
	int xp2 = std::max(t1, t2);
	for(int i = xp1; i < xp2; i++){
		c1->chromosome[i] = p2->chromosome[i];
		c2->chromosome[i] = p1->chromosome[i];
	}
}


void Population::Test()
{
	Individual* p1 = members[0];
	Individual* p2 = members[1];
	Individual* c1 = members[2];
	Individual* c2 = members[3];

}

void Population::OrderCrossover(int s, int e, Individual* p1, Individual* p2, Individual* c1)
{


	for (int i = s; i <= e; i++)
	{
		c1->chromosome[i] = p1->chromosome[i];
	}
	int gene_count = p2->chromLength - (e - s + 1);
	int i = (e + 1) % p2->chromLength;
	int c1_next = i;
	while (gene_count > 0)
	{
		bool exists = false;
		for (int j = s; j <= e; j++)
		{
			if (p1->chromosome[j] == p2->chromosome[i])
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			c1->chromosome[c1_next] = p2->chromosome[i];
			c1_next = (c1_next + 1) % p2->chromLength;
			gene_count--;
		}

		i = (i + 1) % p2->chromLength;
	}

}
