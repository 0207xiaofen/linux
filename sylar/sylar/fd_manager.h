#ifndef __SYLAR_FDMANAGER_H__
#define __SYLAR_FDMANAGER_H__

#include<memory>
#include"thread.h"
#include"singleton.h"

namespace sylar{

class FdCtx : public std::enable_shared_from_this<FdCtx> {
public:
    typedef std::shared_ptr<FdCtx> ptr;

    FdCtx(int fd);
    ~FdCtx();

    bool init();
    bool isInit() const { return m_isInit; }
    bool isSocket() const { return m_isSocket; }
    bool isClosed() const { return m_isClosed; }
    bool closed();

    void setUserNOblock(bool val) { m_userNoBlock = val; }
    bool getUserNoblock() const { return m_userNoBlock; }

    void setsysNoblock(bool val) { m_sysNoBlock = val; }
    bool getsysNoblock() const { return m_sysNoBlock; }

    void setTimeOut(int type, uint64_t v);
    uint64_t getTimeOut(int type);
private:
    bool m_isInit: 1;
    bool m_isSocket: 1;
    bool m_sysNoBlock: 1;
    bool m_userNoBlock: 1;
    bool m_isClosed: 1;
    int m_fd;
    uint64_t m_sendTimeout;
    uint64_t m_recvTimeout;
};

class FdManager{
public:
    typedef RMMutex RWmutexType;
    FdManager();

    FdCtx::ptr get(int fd, bool auto_create = false);
    void del(int fd);
private:
    RWmutexType m_mutex;
    std::vector<FdCtx::ptr> m_datas;
};

typedef Singleton<FdManager> Fdmgr;

}


#endif