#include "mthreadsmgr.hpp"


MThreadsMgr& MThreadsMgr::instance()
{
	static MThreadsMgr mthreadManager;
	return mthreadManager;
}


auto MThreadsMgr::getMThreads(const std::string& threadName)
	-> std::shared_ptr<MThreads>
{
	std::lock_guard<std::mutex> locker(map_mutex);

	auto iter = threadNameMap.find(threadName);

	return (threadNameMap.end() != iter) ? iter -> second : NULL;
}

auto MThreadsMgr::createMThreads(const std::string& threadName, size_t  size)
		-> std::shared_ptr<MThreads>
{
	std::lock_guard<std::mutex> locker(map_mutex);

	auto iter = threadNameMap.find(threadName);

	if(threadNameMap.end() != iter){
		return NULL;
	}

	auto mthreads = std::make_shared<MThreads>(threadName, size);
	threadNameMap[threadName]= mthreads;
	return mthreads;
	
}

auto MThreadsMgr::getCurrentMThreads()
		-> std::shared_ptr<MThreads>
{
	std::string threadName = MThreads::getThreadNameOfCaller();
	std::lock_guard<std::mutex> locker(map_mutex);

	auto iter = threadNameMap.find(threadName);
	return ( threadNameMap.end() != iter 
		&& iter ->second -> containId(std::this_thread::get_id()) )
		? iter->second
		: NULL;
}

void MThreadsMgr::asyncReleaseMThreads(const std::string& threadName)
{
	std::lock_guard<std::mutex> locker(map_mutex);

	auto iter = threadNameMap.find(threadName);

	if(threadNameMap.end() != iter)
	{
		threadNameMap.erase(iter);
	}
}

void MThreadsMgr::asyncReleaseAllMThreads()
{
	std::lock_guard<std::mutex> locker(map_mutex);
	auto iter = threadNameMap.begin();

	while(iter != threadNameMap.end()){
		auto todel = iter++;
		threadNameMap.erase(todel);
	}
}


bool MThreadsMgr::isCurrentInMThreads(const std::string& threadName)
{
	std::lock_guard<std::mutex> locker(map_mutex);
	auto iter = threadNameMap.find(threadName);

	return (threadNameMap.end() != iter 
		&& iter ->second -> containId( std::this_thread::get_id()) );

}

