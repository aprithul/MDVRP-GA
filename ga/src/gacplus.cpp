//============================================================================
// Name        : gacplus.cpp
// Author      : Sushil J Louis
// Version     :
// Copyright   : Copyright University of Nevada, Reno
// Description : Hello World in C++, Ansi-style
//============================================================================
// quick_example.cpp

#ifndef WIN32
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

#include <ctime>
#include "GA.h"
#include <iostream>
//using namespace emscripten;

GA* ga;

#ifdef WIN32
int main(){
	const clock_t begin_time = clock();
	int count = 1;
	while(count >0){
	ga = new GA("");
	ga->SetupOptions();
#else
int RunGA(std::string _data, int gen, float px, float pm) {
	ga = new GA(_data);
	ga->SetupOptions(gen, px, pm);
#endif
	ga->Init();

#ifdef WIN32
	ga->Run();
	std::cout <<"Time taken : "<< float( clock () - begin_time ) /  CLOCKS_PER_SEC;
	delete ga;
	count--;
	//std::cout<<"count: "<<count<<std::endl;
}
#endif

	return 0;
}

#ifndef WIN32
void ga_run_caller()
{
	ga->Run();
}

void delete_ga()
{
	if(ga != nullptr)
		delete ga;
}

EMSCRIPTEN_BINDINGS(my_module) {
	emscripten::function("RunGA", &RunGA);
    emscripten::function("ga_run_caller", &ga_run_caller);
    emscripten::function("delete_ga", &delete_ga);
	
}
#endif