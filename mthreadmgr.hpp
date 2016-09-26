#ifndef _M_THREAD_MGR_H_
#define _M_THREAD_MGR_H_

#include "mthread.hpp"

class  MThreadManager  
{
public:

	static CCmThreadManager* instance();

	MThreads* getMThreads(const std::string& threadName);

	MThreads* createMThreads(const std::string& threadName, size_t  size= 1);


public:
	
	void joinAndStopAll();

	void joinAndStopMThread(const std::string& threadName);

	bool isCurrentInMThread(const std::string& threadName);

private:

	map<std::string, MThreads*> threadNameMap;

	std::mutex map_mutex;
};


#endif
