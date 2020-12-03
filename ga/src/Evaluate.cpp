/*
 * Evaluate.cpp
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */


#include "Evaluate.h"
#include "Utils.h"
#include <math.h>
#include <iostream>
#include <assert.h>

#ifndef WIN32
#include <emscripten.h>

EM_JS(void, clear_canvas, (), {
	clear_canvas();
});

EM_JS(void, draw_node, (double x, double y, int is_depot), {
	draw_node(x,y,is_depot);
});

EM_JS(void, draw_edge, (double x1, double y1, double x2, double y2), {
	draw_edge(x1, y1, x2, y2);
});

#endif



int get_depot_id_from_customer_index(int cust_index)
{

	for(auto depot:depots)
	{
		cust_index-=depot.customer_count;
		if(cust_index<0)
			return depot.id;
	}
	return -1;
}

void draw_edge_js(double x1, double y1, double x2, double y2)
{
	#ifndef WIN32
	draw_edge(x1*10, y1*10, x2*10, y2*10);
	#endif
}

float Calculate_route_len(Individual* individual, bool draw_in_js)
{
	#ifndef WIN32
		if(draw_in_js){
			clear_canvas();
		
			for(auto d: depots)
			{
				draw_node(d.x*10, d.y*10, true);
			}

			for(auto c:customers)
			{
				draw_node(c.x*10, c.y*10, false);
			}
		}
	#endif

	auto cust = customers[individual->chromosome[0]-1];
	auto depot = depots[get_depot_id_from_customer_index(0)-1];
	float route_length = eucledian_distance( depot.x, depot.y, cust.x, cust.y);	//from to depot to first customer's pos
	if(draw_in_js)
		draw_edge_js(depot.x, depot.y, cust.x, cust.y);
	float remaining_cap = depot.capacity - cust.demand;
//std::cout<< "Start depot: "<<depot.id<<std::endl;
//std::cout<<cust.id<<" : "<<cust.demand<<std::endl;
	int i=1;
	while(i < individual->chromLength)
	{
		auto last_cust = cust;
		cust = customers[individual->chromosome[i]-1];

		if( get_depot_id_from_customer_index(i) == depot.id )
		{
			if(remaining_cap - cust.demand >= 0)
			{
				remaining_cap -= cust.demand;
				route_length += eucledian_distance( last_cust.x, last_cust.y, cust.x, cust.y);
				if(draw_in_js)
					draw_edge_js(last_cust.x, last_cust.y, cust.x, cust.y);
			}
			else
			{
//std::cout<< "depot: "<<depot.id<<std::endl;
				route_length +=eucledian_distance( depot.x, depot.y, last_cust.x, last_cust.y);
				if(draw_in_js)
					draw_edge_js( depot.x, depot.y, last_cust.x, last_cust.y);
				route_length +=eucledian_distance( depot.x, depot.y, cust.x, cust.y);
				if(draw_in_js)
					draw_edge_js( depot.x, depot.y, cust.x, cust.y);
				remaining_cap = depot.capacity - cust.demand;
			}

		}
		else
		{
			route_length +=eucledian_distance( depot.x, depot.y, last_cust.x, last_cust.y);
			if(draw_in_js)
				draw_edge_js( depot.x, depot.y, last_cust.x, last_cust.y);
//std::cout<< "End depot: "<<depot.id<<std::endl;
			depot = depots[get_depot_id_from_customer_index(i)-1];
//std::cout<< "Start depot: "<<depot.id<<std::endl;
			route_length +=eucledian_distance( depot.x, depot.y, cust.x, cust.y);
			if(draw_in_js)
				draw_edge_js( depot.x, depot.y, cust.x, cust.y);			
			remaining_cap = depot.capacity - cust.demand;
		}
//std::cout<<cust.id<<" : "<<cust.demand<<std::endl;

		i++;
	}
	route_length +=eucledian_distance( depot.x, depot.y, cust.x, cust.y);
	if(draw_in_js)
		draw_edge_js( depot.x, depot.y, cust.x, cust.y);			
//std::cout<< "End depot: "<<depot.id<<std::endl;
//assert(false);
	return route_length;
}

EvaluationResult Eval(Individual *individual){

	float route_length = Calculate_route_len(individual, false);

	/*int depot_ind = 0;
	double remaining_cap = depots[depot_ind].capacity;
	double route_length = 0;
	int vc[4] = {};
	for (int i = 0; i < individual->chromLength;)
	{
		int cust_id = individual->chromosome[i];
		remaining_cap -= customers[cust_id - 1].demand;
		if (remaining_cap < 0)
		{
			vc[depot_ind]++;
			depot_ind = (depot_ind + 1) % depots.size();
			remaining_cap = depots[depot_ind].capacity;
		}
		else {
			auto _dist = eucledian_distance(depots[depot_ind].x, depots[depot_ind].y, customers[cust_id - 1].x, customers[cust_id - 1].y);
			assert(!std::isnan(_dist));
			route_length += _dist;
			i++;
		}
	}*/

	//std::cout << route_length << std::endl;
	//double sum = 0;
	//for(int i = 0; i < individual->chromLength; i++){
	//		sum += (individual->chromosome[i] == 1 ? 1: 0);
	//}
	EvaluationResult eval {4000 - route_length, route_length};
	return eval;


}
