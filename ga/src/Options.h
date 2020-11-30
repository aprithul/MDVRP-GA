/*
 * Options.h
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

typedef struct {
	std::string infile;
	std::string outfile;

	long int randomSeed;
	int popSize;
	int chromLength;
	unsigned int maxgens;
	float px;
	float pm;

} Options;

#endif /* OPTIONS_H_ */
