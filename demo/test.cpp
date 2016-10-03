
#include <iostream>
#include "../mthreads/mthreadsmgr.hpp"
#include <vector>
#include <chrono>
#include <assert.h>

int example1(){
	MThreadsMgr& mthreadsmgr = MThreadsMgr::instance();
	auto mthreads = mthreadsmgr.createMThreads("example1");

	std::vector<std::future<int> > results;
	std::vector<std::string> output;
	results.emplace_back(
		mthreads->enqueue([&output]{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			output.push_back("1");
			return 1;
		})

	);
	results.emplace_back(
		mthreads->enqueue([&output]{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			output.push_back("2");
			return 2;
		})
	);
	
	assert(results[0].get() == 1);
	assert(results[1].get() == 2);

	assert(output[0] == "1");
	assert(output[1] == "2");
	
	return 0;
}

int example2(){
	MThreadsMgr& mthreadsmgr = MThreadsMgr::instance();
	auto mthreads = mthreadsmgr.createMThreads("example2",2);

	std::vector<std::future<int> > results;
	std::vector<std::string> output;
	results.emplace_back(
		mthreads->enqueue([&output]{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			output.push_back("1");
			return 1;
		})

	);
	results.emplace_back(
		mthreads->enqueue([&output]{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			output.push_back("2");
			return 2;
		})
	);
	
	assert(results[0].get() == 1);
	assert(results[1].get() == 2);

	assert(output[0] == "2");
	assert(output[1] == "1");
	
	return 0;
}



int main(){
	std::cout<<"[  1 ]test mthreads with one thread." << std::endl;
		example1();
	std::cout<<"[ OK ]"<<std::endl;

	std::cout<<"[  2 ]test mthreads with two threads." << std::endl;
		example2();
	std::cout<<"[ OK ]"<<std::endl;
	return 0;
}
