#include"hook.h"
#include<dlfcn.h>
#include"fiber.h"
#include"iomanager.h"
#include"timer.h"
#include"log.h"
#include"fd_manager.h"

namespace sylar{
static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

static thread_local bool t_hook_enable = false;

#define HOOK_FUN(XX)\
        XX(sleep)\
        XX(usleep)\
        XX(nanosleep)\
        XX(socket)\
        XX(connect)\
        XX(accept)\
        XX(read)\
        XX(readv)\
        XX(recvfrom)\
        XX(recv)\
        XX(recvmsg)\
        XX(write)\
        XX(writev)\
        XX(send)\
        XX(sendto)\
        XX(sendmsg)\
        XX(ioctl)\
        XX(fcntl)\
        XX(getsockopt)\
        XX(setsockopt)\
        XX(close)

void hook_init(){
    static bool is_init = false;
    if(is_init){
        return;
    }
#define XX(name) name ## _f = (name ## _fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX);
#undef XX
}

struct _HookIniter{
    _HookIniter(){
        hook_init();
    }
};
static _HookIniter s_hook_initer;

bool is_hook_enable(){
    return t_hook_enable;
}

void set_hook_enable(bool flag){
    t_hook_enable = flag;
}
}

struct timer_info{
    int cancelled = 0;
};
template<typename OriginFun, typename ... Args>
static ssize_t do_io(int fd, OriginFun fun, const char* hook_fun_name
        ,uint32_t event, int time_so, Args&&... args){
    if(!sylar::t_hook_enable){
        return fun(fd, std::forward<Args>(args)...);
    }

    sylar::FdCtx::ptr ctx = sylar::Fdmgr::getInstance()->get(fd);
    if(!ctx){
        return fun(fd, std::forward<Args>(args)...);
    }

    if(ctx->isClosed()){
        errno = EBADF;
        return -1;
    }

    if(!ctx->isSocket() || ctx->getUserNoblock()){
        return fun(fd, std::forward<Args>(args)...);
    }

    uint64_t to = ctx->getTimeOut(time_so);
    std::shared_ptr<timer_info> tinfo(new timer_info);

retry:
    ssize_t n = fun(fd, std::forward<Args>(args)...);
    while(n == (ssize_t)-1 && errno == EINTR){
        n = fun(fd, std::forward<Args>(args)...);
    }
    if(n == (ssize_t)-1 && errno == EAGAIN){
        sylar::IOManager* iom = sylar::IOManager::GetThis();
        sylar::Timer::ptr timer;
        std::weak_ptr<timer_info> winfo(tinfo);

        if(to != (uint64_t)-1){
            timer = iom->addConditionTimer(to, [winfo, fd, iom, event](){
                auto t = winfo.lock();
                if(!t || t->cancelled){
                    return;
                }
                t->cancelled = ETIMEDOUT;
                iom->delEvent(fd, (sylar::IOManager::Event)event); 
            }, winfo);
        }

        int ret = iom->addEvent(fd, (sylar::IOManager::Event)event);
        if(ret == -1){
            SYLAR_LOG_ERROR(sylar::g_logger) << hook_fun_name << " addevent"
                << fd << ", " << event << " )";
            if(timer){
                timer->cancel();
            }
            return -1;
        }else{
            sylar::Fiber::YieldToHold();
            if(timer){
                timer->cancel();
            }
            if(tinfo->cancelled){
                errno = tinfo->cancelled;
                return -1;
            }

            goto retry;
        }
    }

    return n;
    
}

extern "C"{
#define XX(name) name ## _fun name ## _f = nullptr;
    HOOK_FUN(XX);
#undef XX

unsigned int sleep(unsigned int seconds){
    if(!sylar::is_hook_enable()){
        return sleep_f(seconds);
    }
    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();
    iom->addTimer(seconds * 1000, std::bind((void(sylar::Scheduler::*)
        (sylar::Fiber::ptr, int thread)) &sylar::IOManager::schedule, iom, fiber, -1));
    // iom->addTimer(seconds * 1000, [iom, fiber](){
    //     iom->schedule(fiber);
    // });
    sylar::Fiber::YieldToHold();
    return 0;
}

int usleep(useconds_t usec){
    if(!sylar::is_hook_enable()){
        return usleep_f(usec);
    }
    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();
    iom->addTimer(usec / 1000, std::bind((void(sylar::Scheduler::*)
        (sylar::Fiber::ptr, int thread)) &sylar::IOManager::schedule, iom, fiber, -1));
    // iom->addTimer(usec / 1000, [iom, fiber](){
    //     iom->schedule(fiber);
    // });
    sylar::Fiber::YieldToHold();
    return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem){
    if(!sylar::is_hook_enable()){
        return nanosleep_f(req, rem);
    }

    int time_ms = req->tv_sec *1000 + req->tv_nsec / 1000 / 1000;
    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();
    iom->addTimer(time_ms, [iom, fiber](){
        iom->schedule(fiber);
    });
    sylar::Fiber::YieldToHold();
    return 0;
}


int socket(int domain, int type, int protocol){
    if(!sylar::t_hook_enable){
        return socket_f(domain, type, protocol);
    }

    int fd = socket_f(domain, type, protocol);
    if(fd == -1){
        return fd;
    }
    sylar::Fdmgr::getInstance()->get(fd, true);
    return fd;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    return connect_f(sockfd, addr, addrlen);
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int fd = do_io(sockfd, accept_f, "accpet", sylar::IOManager::READ, SO_RCVTIMEO, addr, addrlen);
    if(fd > 0){
        sylar::Fdmgr::getInstance()->get(fd, true);
    }

    return fd;
}


ssize_t read(int fd, void *buf, size_t count){
    return do_io(fd, read_f, "read", sylar::IOManager::READ, SO_RCVTIMEO, buf, count);
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt){
    return do_io(fd, readv_f, "readv", sylar::IOManager::READ, SO_RCVTIMEO, iov, iovcnt);
}


ssize_t recv(int sockfd, void *buf, size_t len, int flags){
    return do_io(sockfd, recv_f, "recv", sylar::IOManager::READ, SO_RCVTIMEO, buf, len, flags);
}


ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){
    return do_io(sockfd, recvfrom_f, "recvform", sylar::IOManager::READ, SO_RCVTIMEO, buf, len, flags, src_addr, addrlen);
}


ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags){
    return do_io(sockfd, recvmsg_f, "recvmsg", sylar::IOManager::READ, SO_RCVTIMEO, msg, flags);
}


ssize_t write(int fd, const void *buf, size_t count){
    return do_io(fd, write_f, "write", sylar::IOManager::WRITE, SO_SNDTIMEO, buf, count);
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt){
    return do_io(fd, writev_f, "writev", sylar::IOManager::WRITE, SO_SNDTIMEO, iov, iovcnt);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags){
    return do_io(sockfd, send_f, "send", sylar::IOManager::WRITE, SO_SNDTIMEO, buf, len, flags);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen){
    return do_io(sockfd, sendto_f, "sendto", sylar::IOManager::WRITE, SO_SNDTIMEO, buf, len, flags, dest_addr, addrlen);
}

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags){
    return do_io(sockfd, sendmsg_f, "sendmsg", sylar::IOManager::WRITE, SO_SNDTIMEO, msg, flags);
}


int close(int fd){
    if(!sylar::is_hook_enable){
        return close_f(fd);
    }

    sylar::FdCtx::ptr ctx = sylar::Fdmgr::getInstance()->get(fd);
    if(ctx){
        auto iom = sylar::IOManager::GetThis();
        if(iom){
            iom->cancelAll(fd);
        }
        sylar::Fdmgr::getInstance()->del(fd);
    }

    return close_f(fd);
}

}

