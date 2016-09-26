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
	
	void joinAllMThreads();

	bool isCurrentThread(const std::string& threadName);

private:

	void registerThread(const std::string& threadName, const MThreads* mthreads);

	void unregisterThread(const std::string& threadName);

	map<std::string, MThreads*> mthreadMap;

	std::mutex map_mutex;
};


#endif
