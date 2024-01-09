#include"util.h"
#include<stdint.h>
#include"log.h"
#include"fiber.h"
#include<sys/time.h>

namespace sylar{
sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

pid_t getThreadId(){
    return syscall(SYS_gettid);
}

uint32_t getFiberId(){
    return Fiber::GetFiberID();
}

void Backtrace(std::vector<std::string>& bt, int size, int skip){
    void** array = (void**)malloc(sizeof(void*) * size);
    size_t s = ::backtrace(array, size);
    char** strings = ::backtrace_symbols(array, s);
    if(strings == nullptr){
        SYLAR_LOG_ERROR(g_logger) << "backtrace_symbols error";
        return;
    }
    for(size_t i = skip; i < s; ++i){
        bt.push_back(strings[i]);
    }

    free(strings);
    free(array);
}

std::string BacktraceTostring(int size, int skip, const std::string& prefix){
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for(size_t i = 0; i < bt.size(); ++i){
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

uint64_t getCurrentMs(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000
        + tv.tv_usec / 1000;
}

uint64_t getCurrentUs(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

}