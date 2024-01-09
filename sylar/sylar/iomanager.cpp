#include"iomanager.h"
#include"log.h"
#include"macro.h"
#include<sys/epoll.h>
#include<unistd.h>
#include<fcntl.h>
#include<string>
#include<errno.h>

namespace sylar{

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

IOManager::FDcontext::EventContext& IOManager::FDcontext::getContext(IOManager::Event event){
    switch(event){
        case IOManager::READ:
            return read;
        case IOManager::WRITE:
            return write;
        default:
            SYLAR_ASSERT2(false, "getcontext");
    }
}
void IOManager::FDcontext::resetContext(FDcontext::EventContext& ctx){
    ctx.scheduelr = nullptr;
    ctx.fiber.reset();
    ctx.cb = nullptr;
}

void IOManager::FDcontext::triggerEvent(Event event){
    SYLAR_ASSERT(ev & event);
    ev = (Event)(ev & ~event);
    EventContext& ctx = getContext(event);
    if(ctx.cb){
        ctx.scheduelr->schedule(&ctx.cb);
    }else{
        ctx.scheduelr->schedule(&ctx.fiber);
    }

    ctx.scheduelr = nullptr;
    return;

}

IOManager::IOManager(size_t threads, bool use_caller, const std::string& name)
        :Scheduler(threads, use_caller, name){
    m_epfd = epoll_create(5000);
    SYLAR_ASSERT(m_epfd != -1);

    int ret = pipe(m_tickleFds);
    SYLAR_ASSERT(ret == 0);

    epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = m_tickleFds[0];

    ret = fcntl(m_tickleFds[0], F_SETFL, O_NONBLOCK);
    SYLAR_ASSERT(ret != -1);

    ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_tickleFds[0], &event);
    SYLAR_ASSERT(ret == 0);

    //m_fdcontexts.resize(64);
    contextResize(32);

    start();
}
IOManager::~IOManager(){
    stop();
    close(m_epfd);
    close(m_tickleFds[0]);
    close(m_tickleFds[1]);

    for(size_t i = 0; i < m_fdcontexts.size(); ++i){
        if(m_fdcontexts[i]){
            delete m_fdcontexts[i];
        }
    }
}

void IOManager::contextResize(size_t size){
    m_fdcontexts.resize(size);

    for(size_t i = 0; i < m_fdcontexts.size(); ++i){
        if(!m_fdcontexts[i]){
            m_fdcontexts[i] = new FDcontext;
            m_fdcontexts[i]->fd = i;
        }
    }
}

int IOManager::addEvent(int fd, Event event, std::function<void()> cb){
    FDcontext* fd_ctx = nullptr;
    RWmutexType::ReadLock lock(m_mutex);
    if(m_fdcontexts.size() > fd){
        fd_ctx = m_fdcontexts[fd];
        lock.unlock();
    }else{
        lock.unlock();
        RWmutexType::WriteLock lock2(m_mutex);
        contextResize(fd * 1.5);
        fd_ctx = m_fdcontexts[fd];
    }

    FDcontext::MutexType::Lock lock2(fd_ctx->mutex);
    if(fd_ctx->ev & event){
        SYLAR_LOG_ERROR(g_logger) << "addEvent assert fd=" << fd
                    << " event=" << event
                    << " fd_ctx.event=" << fd_ctx->ev;
        SYLAR_ASSERT(!(fd_ctx->ev & event));
    }

    int op = fd_ctx->ev ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    epoll_event epevent;
    epevent.events = fd_ctx->ev | EPOLLET | event;
    epevent.data.ptr = fd_ctx;

    int ret = epoll_ctl(m_epfd, op, fd, &epevent);
    if(ret){
        SYLAR_LOG_ERROR(g_logger) << "epoll_ctl(" << m_epfd << ","
            << op << "," << fd << "," << epevent.events << "):"
            << ret << " (" << errno << ") (" << strerror(errno) << ")";
        return -1;
    }

    ++m_pendingEventCount;
    fd_ctx->ev = (Event)(fd_ctx->ev | event);
    FDcontext::EventContext& event_ctx = fd_ctx->getContext(event);
    SYLAR_ASSERT(!event_ctx.cb && !event_ctx.fiber && !event_ctx.scheduelr);

    event_ctx.scheduelr = Scheduler::getThis();
    if(cb){
        event_ctx.cb.swap(cb);
    }else{
        event_ctx.fiber = Fiber::GetThis();
        SYLAR_ASSERT(Fiber::GetThis()->getState() == Fiber::EXEC);
    }

    return 0;
}

bool IOManager::delEvent(int fd, Event event){
    RWmutexType::ReadLock lock(m_mutex);
    if(m_fdcontexts.size() <= fd){
        return false;
    }
    FDcontext* fd_ctx = m_fdcontexts[fd];
    lock.unlock();

    FDcontext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!(fd_ctx->ev & event)){
        return false;
    }

    Event new_event = (Event)(fd_ctx->ev & ~event);
    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = EPOLLET | new_event;
    epevent.data.ptr = fd_ctx;

    int ret = epoll_ctl(m_epfd, op, fd, &epevent);
    if(ret){
        SYLAR_LOG_ERROR(g_logger) << "epoll_ctl(" << m_epfd << ","
            << op << "," << fd << "," << epevent.events << "):"
            << ret << " (" << errno << ") (" << strerror(errno) << ")";
        return false;
    }

    --m_pendingEventCount;
    fd_ctx->ev = new_event;
    FDcontext::EventContext& event_ctx = fd_ctx->getContext(event);
    fd_ctx->resetContext(event_ctx);

    return true;
}

bool IOManager::cancleEvent(int fd, Event event){
    RWmutexType::ReadLock lock(m_mutex);
    if(m_fdcontexts.size() <= fd){
        return false;
    }
    FDcontext* fd_ctx = m_fdcontexts[fd];
    lock.unlock();

    FDcontext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!(fd_ctx->ev & event)){
        return false;
    }

    Event new_event = (Event)(fd_ctx->ev & ~event);
    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = EPOLLET | new_event;
    epevent.data.ptr = fd_ctx;

    int ret = epoll_ctl(m_epfd, op, fd, &epevent);
    if(ret){
        SYLAR_LOG_ERROR(g_logger) << "epoll_ctl(" << m_epfd << ","
            << op << "," << fd << "," << epevent.events << "):"
            << ret << " (" << errno << ") (" << strerror(errno) << ")";
        return false;
    }

    //FDcontext::EventContext& event_ctx = fd_ctx->getContext(event);
    fd_ctx->triggerEvent(event);
    --m_pendingEventCount;

    return true;
}

bool IOManager::cancelAll(int fd){
    RWmutexType::ReadLock lock(m_mutex);
    if(m_fdcontexts.size() <= fd){
        return false;
    }
    FDcontext* fd_ctx = m_fdcontexts[fd];
    lock.unlock();

    FDcontext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!fd_ctx->ev){
        return false;
    }

    int op = EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = 0;
    epevent.data.ptr = fd_ctx;

    int ret = epoll_ctl(m_epfd, op, fd, &epevent);
    if(ret){
        SYLAR_LOG_ERROR(g_logger) << "epoll_ctl(" << m_epfd << ","
            << op << "," << fd << "," << epevent.events << "):"
            << ret << " (" << errno << ") (" << strerror(errno) << ")";
        return false;
    }

    if(fd_ctx->ev & READ){
        fd_ctx->triggerEvent(READ);
        --m_pendingEventCount;
    }
    if(fd_ctx->ev & WRITE){
        fd_ctx->triggerEvent(WRITE);
        --m_pendingEventCount;
    }

    SYLAR_ASSERT(fd_ctx->ev == 0);

    return true;
}

IOManager* IOManager::GetThis(){
    return dynamic_cast<IOManager*>(Scheduler::getThis());
}

void IOManager::tickle(){
    if(hasIdleThreads()){
        int ret = write(m_tickleFds[1], "T", 1);
        SYLAR_ASSERT(ret == 1);
    }

}

bool IOManager::stopping(){
    return m_pendingEventCount == 0
        && Scheduler::stopping();
}

void IOManager::onTimerInsertedAtFront(){
    tickle();
}

void IOManager::idle(){
    epoll_event* events = new epoll_event[64]();
    std::shared_ptr<epoll_event> shared_events(events, [](epoll_event* ptr){
        delete[] ptr;
    });

    while(true){
        uint64_t next_time = 0;
        if(stopping()){
            next_time = getNextTimer();
            if(next_time == ~0ull){
                SYLAR_LOG_INFO(g_logger) << "name=" << getName() << "idle stopping exit";
                break;
            }
        }

        int ret = 0;
        do{
            static int MAX_TIMEOUT = 3000;
            if(next_time != ~0ull){
                next_time = next_time > MAX_TIMEOUT ? MAX_TIMEOUT : next_time;
            }else{
                next_time = MAX_TIMEOUT;
            }
            ret = epoll_wait(m_epfd, events, 64, next_time);
            if(ret < 0 && errno == EINTR){

            }else{
                break;
            }
        }while(true);
        std::vector<std::function<void()> > cbs;
        listExpiredCb(cbs);
        if(!cbs.empty()){
            schedule(cbs.begin(), cbs.end());
            cbs.clear();
        }

        for(int i = 0; i < ret; ++i){
            epoll_event& event = events[i];
            if(event.data.fd == m_tickleFds[0]){
                uint8_t dummy;
                while(read(m_tickleFds[0], &dummy, 1) == 1);
                continue;
            }

            FDcontext* fd_ctx = (FDcontext*)event.data.ptr;
            FDcontext::MutexType::Lock lock(fd_ctx->mutex);
            if(event.events & (EPOLLERR | EPOLLHUP)){
                event.events |= (EPOLLIN | EPOLLOUT) | fd_ctx->ev;
            }

            int real_event = NONE;
            if(event.events & EPOLLIN){
                real_event |= READ;
            }
            if(event.events & EPOLLOUT){
                real_event |= WRITE;
            }

            if((fd_ctx->ev & real_event) == NONE){
                continue;
            }

            int left_event = fd_ctx->ev & (~real_event);
            int op = left_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
            event.events = EPOLLET | left_event;
            int ret2 = epoll_ctl(m_epfd, op, fd_ctx->fd, &event);
            if(ret2){
                SYLAR_LOG_ERROR(g_logger) << "epoll_ctl(" << m_epfd << ","
                    << op << "," << fd_ctx->fd << "," << event.events << "):"
                    << ret2 << " (" << errno << ") (" << strerror(errno) << ")";
                continue;
            }

            if(real_event & READ){
                fd_ctx->triggerEvent(READ);
                --m_pendingEventCount;
            }
            if(real_event & WRITE){
                fd_ctx->triggerEvent(WRITE);
                --m_pendingEventCount;
            }

        }
        Fiber::ptr cur = Fiber::GetThis();
        auto raw_ptr = cur.get();
        cur.reset();

        raw_ptr->swapOut();
    }
    
}

}