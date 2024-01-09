#include"fd_manager.h"
#include"hook.h"
#include <sys/stat.h>

namespace sylar{

FdCtx::FdCtx(int fd)
    :m_isClosed(false)
    ,m_isInit(false)
    ,m_isSocket(false)
    ,m_sysNoBlock(false)
    ,m_userNoBlock(false)
    ,m_fd(fd)
    ,m_sendTimeout(-1)
    ,m_recvTimeout(-1){
        init();
}

FdCtx::~FdCtx(){

}

bool FdCtx::init(){
    if(m_isInit){
        return true;
    }
    m_sendTimeout = -1;
    m_recvTimeout = -1;

    struct stat fd_stat;
    if(-1 == fstat(m_fd, &fd_stat)){
        m_isInit = false;
        m_isSocket = false;
    }else{
        m_isInit = true;
        m_isSocket = S_ISSOCK(fd_stat.st_mode);
    }

    if(m_isSocket){
        int flag = fcntl_f(m_fd, F_GETFL, 0);
        if(!(flag & O_NONBLOCK)){
            fcntl_f(m_fd, F_SETFL, flag | O_NONBLOCK);
        }
        m_sysNoBlock = true;
    }else{
        m_sysNoBlock = false;
    }

    m_userNoBlock = false;
    m_isClosed = false;
    return m_isInit;

}

bool FdCtx::closed(){
    return m_isClosed;
}

void FdCtx::setTimeOut(int type, uint64_t v){
    if(type == SO_RCVTIMEO){
        m_recvTimeout = v;
    }else{
        m_sendTimeout = v;
    }
}

uint64_t FdCtx::getTimeOut(int type){
    if(type == SO_RCVTIMEO){
        return m_recvTimeout;
    }

    return m_sendTimeout;
}

FdManager::FdManager(){
    m_datas.resize(64);
}

FdCtx::ptr FdManager::get(int fd, bool auto_create){
    if(fd == -1){
        return nullptr;
    }
    RWmutexType::ReadLock lock(m_mutex);
    if(m_datas.size() <= fd){
        if(auto_create == false){
            return nullptr;
        }
    }else{
        if(m_datas[fd] || !auto_create){
            return m_datas[fd];
        }
    }
    lock.unlock();
    RWmutexType::WriteLock lock2(m_mutex);
    FdCtx::ptr ctx(new FdCtx(fd));
    if(m_datas.size() <= fd){
        m_datas.resize(fd * 1.5);
    }
    m_datas[fd] = ctx;
    return ctx;
}

void FdManager::del(int fd){
    RWmutexType::WriteLock lock(m_mutex);
    if(m_datas.size() <= fd){
        return;
    }
    m_datas[fd].reset();
}


}