#ifndef _M_THREAD_H_
#define _M_THREAD_H_

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class MThreads {
    friend class MThreadManager;
public:
    MThreads(const std::string& name, size_t  size= 1);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    static std::string&& getThreadNameOfCaller();
private:    
    static int setThreadNameOfCaller(const std::string& threadName);
    
    ~MThreads();
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};


inline MThreads::MThreads(std::string name, size_t  size= 1)
    : stop(false)
{
    for(size_t i = 0;i<size;++i)
        workers.emplace_back(
            [name,this]
            {
                setThreadNameOfCaller(name);
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
auto MThreads::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        //if(stop)
            //throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
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


