/*
 * Utils.h
 *
 *  Created on: Sep 20, 2020
 *      Author: sushil
 */


/*
The format of data and solution files in all directories is as follows:

Data files
The first line contains the following information:

type m n t

type:
0 (VRP)
1 (PVRP)
2 (MDVRP)
3 (SDVRP)
4 (VRPTW)
5 (PVRPTW)
6 (MDVRPTW)
7 (SDVRPTW)
m: number of vehicles
n: number of customers
t: number of days (PVRP), depots (MDVRP) or vehicle types (SDVRP)
The next t lines contain, for each day (or depot or vehicle type), the following information:

D Q

D: maximum duration of a route
Q: maximum load of a vehicle
The next lines contain, for each customer, the following information:

i x y d q f a list e l

i: customer number
x: x coordinate
y: y coordinate
d: service duration
q: demand
f: frequency of visit
a: number of possible visit combinations
list: list of all possible visit combinations
e: beginning of time window (earliest time for start of service), if any
l: end of time window (latest time for start of service), if any
Each visit combination is coded with the decimal equivalent of the corresponding binary bit string. For example, in a 5-day period, the code 10 which is equivalent to the bit string 01010 means that a customer is visited on days 2 and 4. (Days are numbered from left to right.)

Note : In the case of the MDVRP, the lines go from 1 to n + t and the last t entries correspond to the t depots. In the case of the VRP, PVRP and MDVRP, the lines go from 0 to n and the first entry corresponds to the unique depot.*/

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <cmath>
void WriteBufToFile(std::string buf, std::string filename);

float RandomFraction();
int Flip(float prob);
int IntInRange(int low, int high);
double eucledian_distance(double x1, double y1, double x2, double y2);
#endif /* UTILS_H_ */
