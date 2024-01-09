#include"sylar.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_fiber(){
    static int s_conut = 5;
    SYLAR_LOG_INFO(g_logger) << "test in fiber s_conut=" << s_conut;

    sleep(1);
    if(--s_conut >= 0){
        sylar::Scheduler::getThis()->schedule(&test_fiber);
    }
}

int main(int argc, char** argv){
    SYLAR_LOG_INFO(g_logger) << "main";
    sylar::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    sc.schedule(&test_fiber);
    sc.stop();
    SYLAR_LOG_INFO(g_logger) << "end";
    return 0;
}