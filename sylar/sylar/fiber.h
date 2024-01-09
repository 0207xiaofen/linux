#ifndef __SYLAR_FIBER_H__
#define __SYLAR_FIBER_H__

#include<ucontext.h>
#include"util.h"
#include"thread.h"
#include<memory>
//#include"scheduler.h"

namespace sylar{
class Scheduler;
class Fiber : public std::enable_shared_from_this<Fiber> {
friend Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    enum State{
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };
private:
    Fiber();

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);
    ~Fiber();

    //重置协程函数，并重置状态
    //init， term
    void reset(std::function<void()> cb);
    //切换到当前协程执行
    void swapIn();
    //切换到后台执行
    void swapOut();

    void call();
    void back();

    uint64_t getId() const { return m_id; }
    State getState() const { return m_state; }
    void setState(const State s){
        m_state = s;
    }

public:
    //设置当前携程
    static void SetThis(Fiber* f);
    //返回当前协程号
    static Fiber::ptr GetThis();
    //协程切换到后台，并设置为ready状态
    static void YieldToReady();
    //协程切换到后台，并设置为hold状态
    static void YieldToHold();
    //总协程数
    static uint64_t TotalFibers();

    static void MainFunc();
    static void CallerMainFunc();
    static uint64_t GetFiberID();
private:
    uint64_t m_id = 0; //协程id
    uint32_t m_stacksize = 0; //分配的栈的大小
    State m_state = INIT; //协程的状态

    ucontext_t m_ctx;
    void* m_stack = nullptr;

    std::function<void()> m_cb;

};


}

#endif