#ifndef _M_THREAD_MGR_H_
#define _M_THREAD_MGR_H_

#include "mthread.hpp"
#include <memory>
#include <map>

class  MThreadsMgr  
{
public:

	static MThreadsMgr& instance();

	auto getMThreads(const std::string& threadName)
		-> std::shared_ptr<MThreads>;

	auto createMThreads(const std::string& threadName, size_t  size= 1)
		-> std::shared_ptr<MThreads>;

	auto getCurrentMThreads()
		-> std::shared_ptr<MThreads>;

public:
	
	void asyncReleaseAllMThreads();

	void asyncReleaseMThreads(const std::string& threadName);

	bool isCurrentInMThreads(const std::string& threadName);

private:

	std::map<std::string, std::shared_ptr<MThreads> > threadNameMap;

	std::mutex map_mutex;
};


#endif
