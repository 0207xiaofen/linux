#ifndef _SYLAR_THREAD_H_
#define _SYLAR_THREAD_H_

#include<functional>
#include<thread>
#include<string>
#include<memory>
#include<pthread.h>
#include<semaphore.h>
#include<stdint.h>
#include<atomic>

namespace sylar{

class Semaphore{
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();

    void wait();//信号量数量减一
    void notify();//数量加一

private:
    Semaphore(const Semaphore&) = delete;
    Semaphore(const Semaphore&&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;
private:
    sem_t m_semaphore;
};

template<class T>
struct ScopedLockImpl{
public:
    ScopedLockImpl(T& mutex):m_mutex(mutex){
        m_mutex.lock();
        m_locked = true;
    }

    ~ScopedLockImpl(){
        unlock();
    }

    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    T& m_mutex;
    bool m_locked;
};

template<class T>
struct ReadScopedLockImpl{
public:
    ReadScopedLockImpl(T& mutex):m_mutex(mutex){
        m_mutex.rdlock();
        m_locked = true;
    }

    ~ReadScopedLockImpl(){
        unlock();
    }

    void lock(){
        if(!m_locked){
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    T& m_mutex;
    bool m_locked;
};

template<class T>
struct WriteScopedLockImpl{
public:
    WriteScopedLockImpl(T& mutex):m_mutex(mutex){
        m_mutex.wrlock();
        m_locked = true;
    }

    ~WriteScopedLockImpl(){
        unlock();
    }

    void lock(){
        if(!m_locked){
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    T& m_mutex;
    bool m_locked;
};

class Mutex{
public:
    typedef ScopedLockImpl<Mutex> Lock;
    Mutex(){
        pthread_mutex_init(&m_mutex, nullptr);
    }

    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }

    void lock(){
        pthread_mutex_lock(&m_mutex);
    }

    void unlock(){
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

class NullMutex{
public:
    typedef ScopedLockImpl<NullMutex> Lock;
    NullMutex(){}
    ~NullMutex(){}
    void lock(){}
    void unlock(){}
};

class RMMutex{
public:
    typedef ReadScopedLockImpl<RMMutex> ReadLock;
    typedef WriteScopedLockImpl<RMMutex> WriteLock;
    RMMutex(){
        pthread_rwlock_init(&m_lock, nullptr);
    }

    ~RMMutex(){
        pthread_rwlock_destroy(&m_lock);
    }

    void rdlock(){
        pthread_rwlock_rdlock(&m_lock);
    }

    void wrlock(){
        pthread_rwlock_wrlock(&m_lock);
    }

    void unlock(){
        pthread_rwlock_unlock(&m_lock);
    }
private:
    pthread_rwlock_t m_lock;
};

class NullrwMutex{
public:
    typedef ReadScopedLockImpl<NullrwMutex> readlcok;
    typedef WriteScopedLockImpl<NullrwMutex> writelock;
    NullrwMutex(){}
    ~NullrwMutex(){}
    void rdlock(){}
    void wrlock(){}
    void unlock(){}
};

class SpinLock{
public:
    typedef ScopedLockImpl<SpinLock> Lock;
    SpinLock(){
        pthread_spin_init(&m_spin_mutex, 0);
    }

    ~SpinLock(){
        pthread_spin_destroy(&m_spin_mutex);
    }

    void lock(){
        pthread_spin_lock(&m_spin_mutex);
    }

    void unlock(){
        pthread_spin_unlock(&m_spin_mutex);
    }
private:
    pthread_spinlock_t m_spin_mutex;
};

class CASLock{
public:
    typedef ScopedLockImpl<CASLock> Lock;
    CASLock(){
        m_mutex.clear();
    }

    ~CASLock(){

    }

    void lock(){
        while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire));
    }

    void unlock(){
        std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
    }
private:
    volatile std::atomic_flag m_mutex;
};

class Thread{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();

    const std::string& getName() const { return m_name; }

    pid_t getId() const { return m_id; }

    void join();
    static Thread* getThis();
    static const std::string& GetName();
    static void SetName(const std::string& name);

private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;
    static void* run(void* arg);

private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;
    Semaphore m_semaphore;
};

}



#endif