#include"timer.h"
#include<sys/time.h>
#include"util.h"

namespace sylar{

bool Timer::compartator::operator()(const Timer::ptr& lhs, const Timer::ptr& rhs) const{
    if(!lhs && !rhs){
        return false;
    }
    if(!lhs){
        return true;
    }
    if(!rhs){
        return false;
    }
    if(lhs->m_next < rhs->m_next){
        return true;
    }
    if(rhs->m_next < lhs->m_next){
        return false;
    }

    return lhs.get() < rhs.get();
}

Timer::Timer(uint64_t ms, std::function<void()> cb
        ,bool recurring, TimerManager* manager)
        :m_cb(cb)
        ,m_manager(manager)
        ,m_ms(ms)
        ,m_recurring(recurring){
    m_next = getCurrentMs() + m_ms;
}

Timer::Timer(uint64_t next)
    :m_next(next){

}

bool Timer::cancel(){
    TimerManager::RWmutexType::WriteLock lock(m_manager->m_mutex);
    if(m_cb){
        m_cb = nullptr;
        auto it = m_manager->m_timers.find(shared_from_this());
        m_manager->m_timers.erase(it);
        return true;
    }
    return false;
}
bool Timer::refresh(){
    TimerManager::RWmutexType::WriteLock lock(m_manager->m_mutex);
    if(!m_cb){
        return false;
    }

    auto it = m_manager->m_timers.find(shared_from_this());
    if(it == m_manager->m_timers.end()){
        return false;
    }
    m_manager->m_timers.erase(it);
    m_next = getCurrentMs() + m_ms;
    m_manager->m_timers.insert(shared_from_this());

    return true;
}

bool Timer::reset(uint64_t ms, bool from_now){
    if(ms == m_ms && !from_now){
        return true;
    }
    TimerManager::RWmutexType::WriteLock lock(m_manager->m_mutex);
    if(!m_cb){
        return false;
    }

    auto it = m_manager->m_timers.find(shared_from_this());
    if(it == m_manager->m_timers.end()){
        return false;
    }
    m_manager->m_timers.erase(it);

    uint64_t start = 0;
    if(from_now){
        start = getCurrentMs();
    }else{
        start = m_next - m_ms;
    }
    m_ms = ms;
    m_next = start + m_ms;
    m_manager->addTimer(shared_from_this(), lock);
    return true;
}

TimerManager::TimerManager(){
    m_previouseTime = getCurrentMs();
}

TimerManager::~TimerManager(){

}

Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void()> cb
                        , bool recurring){
    Timer::ptr timer(new Timer(ms, cb, recurring, this));
    RWmutexType::WriteLock lock(m_mutex);
    addTimer(timer, lock);
    return timer;
}

static void onTimer(std::weak_ptr<void> weak_cond, std::function<void()> cb){
    std::shared_ptr<void> ptr = weak_cond.lock();
    if(ptr){
        cb();
    }
}

Timer::ptr TimerManager::addConditionTimer(uint64_t ms, std::function<void()> cb
                                ,std::weak_ptr<void> weak_cond, bool recurring){
    return addTimer(ms, std::bind(&onTimer, weak_cond, cb), recurring);
}

uint64_t TimerManager::getNextTimer(){
    RWmutexType::ReadLock lock(m_mutex);
    m_tickled = false;
    if(m_timers.empty()){
        return ~0ull;
    }

    const Timer::ptr& next = *m_timers.begin();
    uint64_t now_time = getCurrentMs();
    if(now_time >= next->m_next){
        return 0;
    }else{
        return next->m_next - now_time;
    }
}

bool TimerManager::noTimer(){
    RWmutexType::ReadLock lock(m_mutex);
    return m_timers.empty();
}

void TimerManager::listExpiredCb(std::vector<std::function<void()> >& cbs){
    uint64_t now_us = getCurrentMs();
    std::vector<Timer::ptr> expired;
    {
        RWmutexType::ReadLock lock(m_mutex);
        if(m_timers.empty()){
            return;
        }
    }
    RWmutexType::WriteLock lock(m_mutex);
    if(m_timers.empty()){
        return;
    }

    bool rollover = detectClockRollover(now_us);
    if(!rollover && (*m_timers.begin())->m_next > now_us){
        return;
    }
    
    Timer::ptr now_timer(new Timer(now_us));
    auto it = rollover ? m_timers.end() : m_timers.lower_bound(now_timer);
    while(it != m_timers.end() && (*it)->m_next == now_us){
        ++it;
    }

    expired.insert(expired.begin(), m_timers.begin(), it);
    m_timers.erase(m_timers.begin(), it);
    cbs.reserve(expired.size());

    for(auto& time : expired){
        cbs.push_back(time->m_cb);
        if(time->m_recurring){
            time->m_next = now_us + time->m_ms;
            m_timers.insert(time);
        }else{
            time->m_cb = nullptr;
        }
    }

}

void TimerManager::addTimer(Timer::ptr val, RWmutexType::WriteLock& lock){
    auto it = m_timers.insert(val).first;
    bool at_first = (it == m_timers.begin()) && !m_tickled;
    if(at_first){
        m_tickled = true;
    }
    lock.unlock();
    if(at_first){
        onTimerInsertedAtFront();
    }
}

bool TimerManager::detectClockRollover(uint64_t now_ms){
    bool rollover = false;
    if(now_ms < m_previouseTime &&
        now_ms < (m_previouseTime - 60 * 60 * 1000)){
        rollover = true;
    }
    m_previouseTime = now_ms;
    return rollover;
}

}