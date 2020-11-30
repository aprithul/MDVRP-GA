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

std::vector<Customer> customers;
std::vector<Depot> depots;

GA::GA(const std::string& _data) {
		
#ifdef WIN32		
		read_map_data("../../C-mdvrp/p01");
#else
		read_map_data(_data);
#endif
	cluster();



	SetupOptions();
	srand(options.randomSeed);

}

GA::~GA() {
	// TODO Auto-generated destructor stub
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
	for (auto& c : customers)
	{
		int _min = INT_MAX;
		for (auto& d : depots)
		{
			double _dist = 0;
			if ( (_dist = eucledian_distance(d.x, d.y, c.x, c.y))< _min)
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
	options.popSize = 300;
	options.chromLength = customers.size();
	options.maxgens = 2000;
	options.px = 0.2f;//0.55f;
	options.pm = 0.4f;
	options.infile = std::string ("infile");
	options.outfile = std::string("outfile");
}

void GA::Init(){
	parent = new Population(options);
	child  = new Population(options);
	parent->Init(); // evaluates, stats, and reports on initial population
	parent->Statistics();
	parent->Report(0);

	//parent->Test();
}


unsigned long int iterations = 1;
void GA::Run(){

#ifdef WIN32
	while(iterations<options.maxgens){
#else
	if(iterations < options.maxgens){
#endif
		parent->Generation(child);
		child->Evaluate();
		child->Statistics();
		child->Report(iterations);

		//parent->LinearRankSelector();

		Population *tmp = parent;
		parent = child;
		child = tmp;

		iterations++;
	}
}