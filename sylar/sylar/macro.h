#ifndef _SYLAR_MACRO_H_
#define _SYLAR_MACRO_H_

#include<string.h>
#include<assert.h>

#define SYLAR_ASSERT(x)\
    if(!(x)){\
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: "#x\
            << "\nbacktrace:\n"\
            << sylar::BacktraceTostring(100, 2, "    ");\
        assert(x);\
    }
#define SYLAR_ASSERT2(x, w)\
    if(!(x)){\
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: "#x\
            << "\n" << w\
            << "\nbacktrace:\n"\
            << sylar::BacktraceTostring(100, 2, "    ");\
        assert(x);\
    }


#endif