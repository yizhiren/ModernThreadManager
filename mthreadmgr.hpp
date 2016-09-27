#ifndef _M_THREAD_MGR_H_
#define _M_THREAD_MGR_H_

#include "mthread.hpp"

class  MThreadManager  
{
public:

	static CCmThreadManager* instance();

	MThreads* getMThreads(const std::string& threadName);

	MThreads* createMThreads(const std::string& threadName, size_t  size= 1);

	MThreads* getCurrentMThreads();

public:
	
	void joinAndReleaseAll();

	void joinAndReleaseMThreads(const std::string& threadName);

	bool isCurrentInMThreads(const std::string& threadName);

private:

	map<std::string, MThreads*> threadNameMap;

	std::mutex map_mutex;
};


#endif
