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

	return (mthreadMap.end() == iter) ? NULL : iter -> second;
}

MThreads* MThreadManager::createMThreads(const std::string& threadName, size_t  size)
{
	std::lock_guard locker(map_mutex);

	map<std::string, MThreads*>::iterator iter =
		mthreadMap.find(threadName);

	if(mthreadMap.end() != iter){
		return NULL;
	}

	MThreads* mthreads = new MThreads(threadName, size);
	threadNameMap[threadName]= mthreads;
	return mthreads;
	
}

void MThreadManager::joinAndStopMThread(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);

	map<std::string, MThreads*>::iterator iter =
		mthreadMap.find(threadName);

	if(mthreadMap.end() != iter)
	{
		delete iter->second;
		mthreadMap.erase(iter);
	}
}

void MThreadManager::joinAndStopAll()
{
	std::lock_guard locker(map_mutex);
	map<std::string, MThreads*>::iterator iter =
		mthreadMap.begin();

	while(iter != mthreadMap.end()){
		map<std::string, MThreads*>::iterator todel = iter++;

		delete todel->second;	
		mthreadMap.erase(todel);
	}
}


bool MThreadManager::isCurrentInMThread(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);
	map<std::string, MThreads*>::iterator iter =
		mthreadMap.find(threadName);

	return (mthreadMap.end() != iter 
		&& iter ->second -> containId(std::this_thread::get_id()) )

}

