/*
 * GA.h
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#ifndef GA_H_
#define GA_H_

#include "Population.h"
#include "Options.h"
#include <string>
#include <time.h>
class GA {

private:
	void Construct();

public:
	GA(const std::string& _data);

	virtual ~GA();

	//--------------------------------
	Population *parent;
	Population *child;
	Options options;
	unsigned long int iterations;

	void SetupOptions();
	void read_map_data(const std::string& _file);
	void cluster();
	void Init();
	void Run();
};

#endif /* GA_H_ */
