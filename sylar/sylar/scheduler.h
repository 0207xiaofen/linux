#ifndef __SYLAR_SCHEDULER_H__
#define __SYLAR_SCHEDULER_H__

#include<memory>
#include"thread.h"
#include"fiber.h"
#include<list>

namespace sylar{

class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    Scheduler(size_t threads = 1, bool use_caller = true, const std::string& name = "");
    virtual ~Scheduler();

    const std::string& getName() const { return m_name; }
    
    static Scheduler* getThis();
    static Fiber* getMainFiber();

    void start();
    void stop();

    template<class FiberOrCb>
    void schedule(FiberOrCb fc, int thread = -1){
        bool need_tickle = false;
        {
            Mutex::Lock lock(m_mutex);
            need_tickle = scheduleNoLock(fc, thread);
        }
        if(need_tickle){
            tickle();
        }
    }

    template<class InputIterator>
    void schedule(InputIterator begin, InputIterator end){
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while(begin != end){
                need_tickle = scheduleNoLock(&*begin, -1) || need_tickle;
                ++begin;
            }
        }
        if(need_tickle){
            tickle();
        }
    }
protected:
    virtual void tickle();
    void run();
    virtual bool stopping();
    virtual void idle();
    void setThis();

    bool hasIdleThreads() { return m_idleThreadCount > 0; }
private:
    template<class FiberOrCb>
    bool scheduleNoLock(FiberOrCb fc, int thread){
        bool need_tickle = m_fibers.empty();
        FiberAndThread ft(fc, thread);
        if(ft.cb || ft.fiber){
            m_fibers.push_back(ft);
        }
        return need_tickle;
    }

private:
    struct FiberAndThread{
        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread; //线程号
        FiberAndThread(Fiber::ptr f, int thr):fiber(f),thread(thr){

        }
        FiberAndThread(Fiber::ptr* f, int thr):thread(thr){
            fiber.swap(*f);
        }
        FiberAndThread(std::function<void()> f, int thr):cb(f), thread(thr){

        }
        FiberAndThread(std::function<void()>* f, int thr):thread(thr){
            cb.swap(*f);
        }
        FiberAndThread():thread(-1){

        }

        void reset(){
            fiber = nullptr;
            cb = nullptr;
            thread = -1;
        }
    };
private:
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads; //线程数组
    std::list<FiberAndThread> m_fibers;  //即将或者计划执行的协程
    std::string m_name;
    Fiber::ptr m_rootFiber; //主协程
protected:
    std::vector<int> m_threadIds; //线程id
    size_t m_threadCount = 0;  //线程数量
    std::atomic<size_t> m_activeThreadCount = {0}; //活跃线程数量
    std::atomic<size_t> m_idleThreadCount = {0};   //空闲线程数量
    bool m_stopping = true;
    bool m_autoshop = false;
    int m_rootThread = 0;  //主线程id
};

}

#endif