#ifndef _M_THREAD_H_
#define _M_THREAD_H_


#include <chrono>
#include <set>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "mtime.hpp"
#include <sys/prctl.h>

class MThreads {
    friend class MThreadManager;
public:
    MThreads(const std::string& name, size_t  size= 1);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    template<class F, class... Args>
    auto enqueue_wait(const MTime& mtime,F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    template<class F, class... Args>
    auto enqueue_until(const MTime& mtime,F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

        

    void pushThreadId(std::thread::id tid);
    bool containId(std::thread::id tid);
    ~MThreads();

    static std::string&& getThreadNameOfCaller();
private:    
    static int setThreadNameOfCaller(const std::string& threadName);
    
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
    //
    std::set<std::thread::id> threadIdSet;
};

inline std::string&& MThreads::getThreadNameOfCaller()
{
    const int MAX_THREAD_NAME_LENGTH = 16;
    const int MAX_THREAD_NAME_LENGTH_ENSURE_ENOUGH_FOR_FUTURE = MAX_THREAD_NAME_LENGTH * 2;
    char threadName[MAX_THREAD_NAME_LENGTH_ENSURE_ENOUGH_FOR_FUTURE] = {0};
    prctl(PR_GET_NAME, (unsigned long)threadName, 0, 0, 0);
    return std::move<std::string>(threadName);
}

inline int MThreads::setThreadNameOfCaller(const std::string& threadName)
{
    return prctl(PR_SET_NAME, (unsigned long)threadName.c_str(), 0, 0, 0);
}

inline void MThreads::pushThreadId(std::thread::id tid)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    threadIdSet.insert(tid);
}

inline bool MThreads::containId(std::thread::id tid)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    return  threadIdSet.find(tid) != threadIdSet.end();
}

inline MThreads::MThreads(const std::string& name, size_t size)
    : stop(false)
{
    for(size_t i = 0;i<size;++i)
        workers.emplace_back(
            [name,this ]
            {
                setThreadNameOfCaller(name);
                this->pushThreadId(std::this_thread::get_id());
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            }
        );
}

template<class F, class... Args>
auto MThreads::enqueue_wait(const MTime& mtime, F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{

    MTime mruntime = mtime + MTime::getTimeOfDay();
    return enqueue_until(mruntime,std::forward<F>(f), std::forward<Args>(args)...);
}

template<class F, class... Args>
auto MThreads::enqueue_until(const MTime& mtime, F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        tasks.emplace([task,mtime](){
            MTime now = MTime::getTimeOfDay();
            if(mtime > now){
                long waittime = (mtime-now).getTotalInMsec();
                std::this_thread::sleep_for(std::chrono::milliseconds(waittime)); 
            }
            (*task)(); 
        });
    }
    condition.notify_one();
    return res;
}


template<class F, class... Args>
auto MThreads::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    return enqueue_wait(MTime::mtZero, std::forward<F>(f), std::forward<Args>(args)...);
}


inline MThreads::~MThreads()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

#endif


