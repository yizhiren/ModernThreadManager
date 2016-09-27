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

	return (mthreadMap.end() != iter) ? iter -> second : NULL;
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

MThreads* MThreadManager::getCurrentMThreads()
{
	std::string threadName = MThreads::getThreadNameOfCaller();
	std::lock_guard locker(map_mutex);

	map<std::string, MThreads*>::iterator iter =
		mthreadMap.find(threadName);
	return ( mthreadMap.end() != iter 
		&& iter ->second -> containId(std::this_thread::get_id()) )
		？ iter->second
		: NULL;
}

void MThreadManager::joinAndReleaseMThreads(const std::string& threadName)
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

void MThreadManager::joinAndReleaseAll()
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


bool MThreadManager::isCurrentInMThreads(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);
	map<std::string, MThreads*>::iterator iter =
		mthreadMap.find(threadName);

	return (mthreadMap.end() != iter 
		&& iter ->second -> containId(std::this_thread::get_id()) )

}

