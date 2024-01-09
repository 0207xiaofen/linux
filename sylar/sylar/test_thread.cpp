#include"thread.h"
#include"log.h"
#include"util.h"
#include"singleton.h"
#include<vector>
#include<unistd.h>
#include"config.h"

int count = 0;

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
sylar::RMMutex s_mutex;
sylar::Mutex mutex;

void func1(){
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                             << " this name: " << sylar::Thread::getThis()->getName()
                             << " id: " << sylar::getThreadId()
                             << " this id: " << sylar::Thread::getThis()->getId();
    for(int i = 0; i < 1000000; ++i){
        //sylar::RMMutex::ReadLock lock(s_mutex);
        sylar::Mutex::Lock lock(mutex);
        count++;
    }
}

void func2(){
    while(1){
        SYLAR_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void func3(){
    while(1){
        SYLAR_LOG_INFO(g_logger) << "==========================================";
    }
}

int main(int agrc, char** argv){
    SYLAR_LOG_INFO(g_logger) << "thread test begin";

    YAML::Node root = YAML::LoadFile("/home/wangxian/program/workspace/sylar/bin/config/log2.yml");
    sylar::Config::LoadFromYaml(root);

    std::vector<sylar::Thread::ptr> thrs;
    for(int i = 0; i < 2; ++i){
        sylar::Thread::ptr thr(new sylar::Thread(&func2, "name_" + std::to_string(i * 2)));
        sylar::Thread::ptr thr1(new sylar::Thread(&func3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        thrs.push_back(thr1);
    }

    for(size_t i = 0; i < thrs.size(); ++i){
        thrs[i]->join();
    }

    SYLAR_LOG_INFO(g_logger) << "thread test end";
    SYLAR_LOG_INFO(g_logger) << "count = " << count;

    
    return 0;
}