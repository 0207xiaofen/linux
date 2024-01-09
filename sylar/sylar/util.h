#ifndef _SYLAR_UTIL_H_
#define _SYLAR_UTIL_H_
#include<thread>
#include<unistd.h>
#include<sys/types.h>
#include<iostream>
#include<sys/syscall.h>
#include<vector>
#include<execinfo.h>

namespace sylar{

pid_t getThreadId();//获取线程id

uint32_t getFiberId();//获取协程Id

void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);

std::string BacktraceTostring(int size = 64, int skip = 2, const std::string& prefix = "");

//时间ms
uint64_t getCurrentMs();
uint64_t getCurrentUs();
}



#endif