#include"sylar.h"
#include"iomanager.h"
#include"log.h"
#include"hook.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_hook(){
    sylar::IOManager iom(1);
    iom.schedule([](){
        sleep(2);
        SYLAR_LOG_INFO(g_logger) << "sleep 2";
    });

    iom.schedule([](){
        sleep(3);
        SYLAR_LOG_INFO(g_logger) << "sleep 3";
    });

    SYLAR_LOG_INFO(g_logger) << "test sleep";
}

int main(int argc, char**argv){
    test_hook();

    return 0;
}