/*
 * GA.cpp
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#include "GA.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Utils.h"
#include <assert.h>
std::vector<Customer> customers;
std::vector<Depot> depots;

GA::GA(const std::string& _data) {

		
#ifdef WIN32		
		read_map_data("../../C-mdvrp/p04");
#else
		read_map_data(_data);
#endif
	cluster();


	//SetupOptions();
	//srand(options.randomSeed);
	//srand(time(NULL));
}

GA::~GA() {
	// TODO Auto-generated destructor stub
	customers.clear();
	depots.clear();
	delete parent;
	delete child;
}



void GA::read_map_data(const std::string& _file)
{
	std::string line;

#ifdef WIN32
	std::ifstream myfile(_file);
	
	if (myfile.is_open())
	{
		//std::cout << "Reading" << std::endl;
		//type m n t
		
#else
		std::stringstream myfile(_file);
#endif
		getline(myfile, line);
		std::stringstream line_stream(line);
		std::string token;
		std::vector<std::string> tokens;
		while (getline(line_stream, token, ' '))
		{
			tokens.push_back(token);
		}
		
		int n_vehicles = stod(tokens[1]);
		int n_customers = stod(tokens[2]);
		int n_depots = stod(tokens[3]);

		std::cout << "vechiles: " << n_vehicles << std::endl;
		std::cout << "customers: " << n_customers<< std::endl;
		std::cout << "depots: " << n_depots << std::endl;
		

		for (int i = 0; i < n_depots; i++)
		{
			tokens.clear();
			getline(myfile, line);
			std::stringstream line_stream(line);
			while (getline(line_stream, token, ' '))
			{
				tokens.push_back(token);
			}
			Depot d;
			d.capacity = stod(tokens[1]);
			depots.push_back(d);
		}

		for (int i = 0; i < n_customers; i++)
		{
			tokens.clear();
			getline(myfile, line);
			std::stringstream line_stream(line);
			while (getline(line_stream, token, ' '))
			{
				if (token.empty())
					continue;
				tokens.push_back(token);
			}
			Customer c = { stoi(tokens[0]), -1, stod(tokens[1]), stod(tokens[2]), stod(tokens[4])};
			customers.push_back(c);
			//std::cout << c.id << ":" << c.x << "," << c.y << std::endl;
		}

		for (int i = 0; i < n_depots; i++)
		{
			tokens.clear();
			getline(myfile, line);
			std::stringstream line_stream(line);
			while (getline(line_stream, token, ' '))
			{
				if (token.empty())
					continue;
				tokens.push_back(token);
			}
			depots[i].id = i+1;//stoi(tokens[0]);
			depots[i].x  = stod(tokens[1]);
			depots[i].y  = stod(tokens[2]);
			depots[i].customer_count = 0;
		}

	#ifdef WIN32
		myfile.close();
	}
	else std::cout << "Unable to open file";
	#endif
}


void GA::cluster()
{
	/*for(auto& c : customers)
	{
		c.depot_id = IntInRange(1, depots.size()+1);
	}*/
	
	for (auto& c : customers)
	{
		int _min = INT_MAX;
		for (auto& d : depots)
		{
			double _dist = 0;
			if ( (_dist = eucledian_distance(d.x, d.y, c.x, c.y)) < _min)
			{
				_min = _dist;
				c.depot_id = d.id;
				//std::cout << c.depot_id << std::endl;
			}
		}
	}

	for(auto c:customers)
	{
		depots[c.depot_id-1].customer_count++;
	}	
}

void GA::SetupOptions(){
	options.randomSeed = 121;
	options.popSize = 1000;
	options.chromLength = customers.size();
	options.maxgens = 250;
	options.px = 0.75;//0.5f;//1.f;//0.2f;//0.55f;
	options.pm = 0.5f;
	options.elit_percent = 0.1f;//0.1f;
	options.infile = std::string ("infile");
	options.outfile = std::string("outfile");
}

void GA::SetupOptions(int gen, float px, float pm){
	options.randomSeed = time(NULL);
	options.popSize = 1000;
	options.chromLength = customers.size();
	options.maxgens = gen;
	options.px = px;//0.5f;//1.f;//0.2f;//0.55f;
	options.pm = pm;
	options.elit_percent = 0.1f;//0.1f;
	options.infile = std::string ("infile");
	options.outfile = std::string("outfile");

	std::cout<<"from c++: "<<px<<","<<pm<<","<<std::endl;
}

void GA::Init(){
	srand(time(NULL));
	parent = new Population(options, nullptr);
	child  = new Population(options, parent);
	parent->Init(); // evaluates, stats, and reports on initial population
	std::cout<<"Init:"<<std::endl;
	parent->Statistics();
	parent->Report(0);
	std::cout<<"end"<<std::endl;
	iterations = 1;
	//parent->Test();
}



static bool population_comp( Individual* a, Individual* b)
{
	return a->fitness>b->fitness;
}

void GA::Run(){

#ifdef WIN32
	while(iterations<options.maxgens){
#else
	if(iterations < options.maxgens){
#endif
		parent->Generation(child);
		//child->Evaluate();	// not needed because iterated swap already evalutes the child during generation
		child->Statistics();
		child->Report(iterations);


		// elitism
		std::sort(parent->members, parent->members+options.popSize, population_comp);
		std::sort(child->members, child->members+options.popSize, population_comp);
		int elit_size = options.popSize * options.elit_percent;
		for(int i=0; i<elit_size; i++)
		{
			parent->members[i]->copy_into( child->members[options.popSize-i-1]);
		}

		Population *tmp = parent;
		parent = child;
		child = tmp;

		iterations++;
	}

	parent->Report_file();
}