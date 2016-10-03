# ModernThreadManager
thread manager based on modern c++11 thread.

## usage
```
	MThreadsMgr& mthreadsmgr = MThreadsMgr::instance();
	auto mthreads = mthreadsmgr.createMThreads("simpleusage",10);
	auto mfuture = mthreads->enqueue([]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		return 1;
	});
	std::cout << mfuture.get() << std::endl;

```


[![Build Status](https://secure.travis-ci.org/yizhiren/ModernThreadManager.png)](http://travis-ci.org/yizhiren/ModernThreadManager)

