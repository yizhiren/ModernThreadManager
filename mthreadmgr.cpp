#include "mthreadmgr.hpp"


MThreadManager& MThreadManager::instance()
{
	static MThreadManager mthreadManager;
	return mthreadManager;
}


MThreads* MThreadManager::getMThreads(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);

	map<std::string, MThreads*>::const_iterator iter =
		mthreadMap.find(threadName);

	if(mthreadMap.end() == iter){
		return NULL;
	} else {
		return iter -> second;
	}
}

MThreads* MThreadManager::createMThreads(const std::string& threadName, size_t  size)
{
	std::lock_guard locker(map_mutex);

	map<std::string, MThreads*>::iterator iter =
		mthreadMap.find(threadName);

	if(mthreadMap.end() == iter){
		MThreads* mthreads = new MThreads(threadName, size);
		registerThread(threadName, mthreads);
	} else {
		return NULL;
	}
	
}

void MThreadManager::registerThread(const std::string& threadName, const MThreads* mthreads)
{
	
}

void MThreadManager::unregisterThread(const std::string& threadName)
{
	
}

void MThreadManager::joinAllMThreads()
{
	std::lock_guard locker(map_mutex);
	map<std::string, MThreads*>::iterator iter =
		mthreadMap.begin();

	while(iter != mthreadMap.end()){
		delete iter->second;
		map<std::string, MThreads*>::iterator todel = iter++;
	
		mthreadMap.erase(todel);
	}
}

class  MThreadManager  
{
public:

	static MThreadManager& instance();

	MThreads* getMThreads(const std::string& threadName);

	MThreads* createMThreads(const std::string& threadName);


public:
	
	int joinAllMThreads();

	int registerThread(MThreads* mThreads);

	int unregisterThread(MThreads* mThreads);


	static bool isCurrentThread(const std::string& threadName);


};