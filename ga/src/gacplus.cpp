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

#include "GA.h"
#include <iostream>
//using namespace emscripten;

GA* ga;

#ifdef WIN32
int main(){
	int count = 10;
	while(count >0){
	ga = new GA("");
#else
int RunGA(std::string _data) {
	ga = new GA(_data);
#endif
	ga->Init();

#ifdef WIN32
	ga->Run();
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