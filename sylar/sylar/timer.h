#ifndef __SYLAR_TIMER_H__
#define __SYLAR_TIMER_H__

#include<memory.h>
//#include"iomanager.h"
#include<vector>
#include"thread.h"
#include<set>

namespace sylar{

class TimerManager;
class Timer: public std::enable_shared_from_this<Timer> {
friend class TimerManager;
public:
    typedef std::shared_ptr<Timer> ptr;
    bool cancel();
    bool refresh();
    bool reset(uint64_t ms, bool from_now);
private:
    Timer(uint64_t ms, std::function<void()> cb
        ,bool recurring, TimerManager* manager);
    Timer(uint64_t next);

private:
    bool m_recurring = false;    //是否循环定时
    uint64_t m_ms = 0;       //循环周期
    uint64_t m_next = 0;     //精准执行时间
    std::function<void()> m_cb;
    TimerManager* m_manager = nullptr;
private:
    struct compartator{
        bool operator()(const Timer::ptr& lhs, const Timer::ptr& rhs) const;
    };
};

class TimerManager{
friend class Timer;
public:
    typedef RMMutex RWmutexType;

    TimerManager();
    virtual ~TimerManager();

    Timer::ptr addTimer(uint64_t ms, std::function<void()> cb
                        , bool recurring = false);
    Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb
                                ,std::weak_ptr<void> weak_cond, bool recurring = false);
    uint64_t getNextTimer();
    void listExpiredCb(std::vector<std::function<void()> >& cbs);   //定时器超时未执行的回调函数列表

    bool noTimer();
protected:
    virtual void onTimerInsertedAtFront() = 0;
    void addTimer(Timer::ptr val, RWmutexType::WriteLock& lock);
private:
    bool detectClockRollover(uint64_t now_ms);
private:
    RWmutexType m_mutex;
    std::set<Timer::ptr, Timer::compartator> m_timers;
    bool m_tickled = false;
    uint64_t m_previouseTime = 0;
};



}


#endif