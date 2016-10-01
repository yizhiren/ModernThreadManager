#include "mthreadmgr.hpp"


MThreadManager& MThreadManager::instance()
{
	static MThreadManager mthreadManager;
	return mthreadManager;
}


auto MThreadManager::getMThreads(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);

	auto iter = mthreadMap.find(threadName);

	return (mthreadMap.end() != iter) ? iter -> second : NULL;
}

auto MThreadManager::createMThreads(const std::string& threadName, size_t  size)
{
	std::lock_guard locker(map_mutex);

	auto iter = mthreadMap.find(threadName);

	if(mthreadMap.end() != iter){
		return NULL;
	}

	auto mthreads = make_shared<MThreads>(threadName, size);
	threadNameMap[threadName]= mthreads;
	return mthreads;
	
}

auto MThreadManager::getCurrentMThreads()
{
	std::string threadName = MThreads::getThreadNameOfCaller();
	std::lock_guard locker(map_mutex);

	auto iter = mthreadMap.find(threadName);
	return ( mthreadMap.end() != iter 
		&& iter ->second -> containId(std::this_thread::get_id()) )
		? iter->second
		: NULL;
}

void MThreadManager::joinAndReleaseMThreads(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);

	auto iter = mthreadMap.find(threadName);

	if(mthreadMap.end() != iter)
	{
		mthreadMap.erase(iter);
	}
}

void MThreadManager::joinAndReleaseAll()
{
	std::lock_guard locker(map_mutex);
	auto iter = mthreadMap.begin();

	while(iter != mthreadMap.end()){
		auto todel = iter++;
		mthreadMap.erase(todel);
	}
}


bool MThreadManager::isCurrentInMThreads(const std::string& threadName)
{
	std::lock_guard locker(map_mutex);
	auto iter = mthreadMap.find(threadName);

	return (mthreadMap.end() != iter 
		&& iter ->second -> containId(std::this_thread::get_id()) )

}

