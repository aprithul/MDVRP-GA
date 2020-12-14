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

double Population::avg = -1;
double Population::avgDist = -1;
double Population::min = -1;
double Population::minDist = -1;
double Population::max = -1;
double Population::sumFitness = -1;
//Individual* Population::best_solution = nullptr;// = new Individual();

Population::Population(Options opts, Population* other) {
	options = opts;
	avg = min = max = sumFitness = -1;
	minDist = INT_MAX;
	
	assert(options.popSize <= MAXPOP);
	for(int i=0; i<5; i++)
	{
		offsprings[i] = new Individual(options.chromLength);
	}

	if(other != nullptr)
	{
		for (int i = 0; i < options.popSize; i++)
		{
			members[i] = new Individual(options.chromLength);
			members[i]->Init(other->members[i]);
		}
	}
	else
	{
		members[0] = new Individual(options.chromLength);
		members[0]->Init();
		for (int i = 1; i < options.popSize; i++)
		{
			members[i] = new Individual(options.chromLength);
			members[i]->Init(members[0]);
			IteratedSwap(members[i]);
			//EvaluationResult res = Eval(members[i]);
			//members[i]->fitness = res.fitness;
			//members[i]->distance = res.distance;
		}
		//EvaluationResult res = Eval(members[0]);
		//members[0]->fitness = res.fitness;
		//members[0]->distance = res.distance;
		IteratedSwap(members[0]);
	}
	

	//if(best_solution == nullptr)
	//{
		best_solution = new Individual(options.chromLength);	
	//}
}

Population::~Population() {
	// TODO Auto-generated destructor stub
	for(int i=0; i<options.chromLength; i++)
		delete members[i];
	for(int i=0; i<5; i++)
		delete offsprings[i];

	if(best_solution != nullptr)
		delete best_solution;
	best_solution = nullptr;
}

void Population::Init(){
	Evaluate();
}

void Population::Evaluate(){
	for (int i = 0; i < options.popSize; i++){
		auto res = Eval(members[i]);
		members[i]->fitness = res.fitness;
		members[i]->distance = res.distance;
	}
}

void Population::Statistics(){
	sumFitness = 0;
	float sumDistance = 0;
	min = members[0]->fitness;
	max = members[0]->fitness;
	auto _last = minDist;
	int best_ind = -1;
	for(int i = 0; i < options.popSize; i++){
		sumFitness += members[i]->fitness;
		sumDistance+= members[i]->distance;
		if(min > members[i]->fitness)
			min = members[i]->fitness;
		if(max < members[i]->fitness)
			max = members[i]->fitness;
		if(members[i]->distance < minDist){
			minDist = members[i]->distance;
			best_ind = i;
		}
	}
	//std::cout<<_last<<","<<minDist<<std::endl;
	avg = sumFitness/options.popSize;
	avgDist = sumDistance/options.popSize;
	if(best_ind != -1)
	{
		members[best_ind]->copy_into(best_solution);
		Calculate_route_len(best_solution, true, nullptr);
		if(best_solution->distance < 473)
		{
			for(int i=0; i<best_solution->chromLength;i++)
			{
				std::cout<<best_solution->chromosome[i]<<"-";
			}
			std::cout<<std::endl;
			assert(false);
		}

	}
}

#ifndef WIN32
EM_JS(void, call_js_update_chart, (double best, double avg), {
	update_chart(best, avg);
});
#endif

void Population::Report(unsigned long int gen){
#ifdef WIN32
	char printbuf[1024];
	sprintf(printbuf, "%4i \t %f \t %f \t %f \t %f %f\n ", (int)gen, min, avg, max, avgDist, minDist);
	//WriteBufToFile(std::string(printbuf), options.outfile);
	
	std::cout << printbuf;
#else
	call_js_update_chart(minDist, avgDist);
#endif
}

void Population::Report_file()
{
	char printbuf_2[128];
	sprintf(printbuf_2, "%f\n", minDist);
	WriteBufToFile(std::string(printbuf_2), options.outfile);
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
		EvaluationResult res = Eval(c1);
		//c1->fitness = res.fitness;
		//c1->distance = res.distance;
		//res = Eval(c2);
		//c2->fitness = res.fitness;
		//c2->distance = res.distance;

		IteratedSwap(c1);
		IteratedSwap(c2);
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
	int indices[10];

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

void Population::IteratedSwap(Individual* individual)
{
	int count = 100;
	EvaluationResult res = Eval(individual);
	individual->fitness = res.fitness;
	individual->distance = res.distance;

	while(true)
	{
		int pos_1 = IntInRange(0, options.chromLength);
		int pos_2 = IntInRange(0, options.chromLength);

		if(pos_1 > pos_2)
			std::swap(pos_1, pos_2);

		individual->copy_into(offsprings[0]);
		std::swap(offsprings[0]->chromosome[pos_1], offsprings[0]->chromosome[pos_2]);

		offsprings[0]->copy_into(offsprings[1]);
		std::swap(offsprings[1]->chromosome[pos_1], offsprings[1]->chromosome[ (options.chromLength+ pos_1-1)%options.chromLength]);

		offsprings[0]->copy_into(offsprings[2]);
		std::swap(offsprings[2]->chromosome[pos_1], offsprings[2]->chromosome[ (options.chromLength+ pos_1+1)%options.chromLength]);

		offsprings[0]->copy_into(offsprings[3]);
		std::swap(offsprings[3]->chromosome[pos_2], offsprings[3]->chromosome[ (options.chromLength+ pos_2-1)%options.chromLength]);

		offsprings[0]->copy_into(offsprings[4]);
		std::swap(offsprings[4]->chromosome[pos_2], offsprings[4]->chromosome[ (options.chromLength+ pos_2+1)%options.chromLength]);

		bool found_better = false;
		for(int i=0; i<5;i++)
		{
			auto off = offsprings[i];
			EvaluationResult res = Eval(off);
			off->fitness = res.fitness;
			off->distance = res.distance;
			if(off->fitness > individual->fitness)
			{
				found_better = true;
				off->copy_into(individual);
			}
		}


		if(!found_better)
			break;

		count--;
		assert(count>0);
	}
}