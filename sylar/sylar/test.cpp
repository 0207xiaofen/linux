#include <iostream>
#include "log.h"
#include"util.h"
#include<thread>

int main(int argc, char** argv) {
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppend));

    sylar::FileLogAppend::ptr file_appender(new sylar::FileLogAppend("./log.txt"));

    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);

    logger->addAppender(file_appender);

    //sylar::LogEvent::ptr event(new sylar::LogEvent(logger, __FILE__, __LINE__, 0, sylar::getThreadId(), sylar::getFiberId(), time(0)));
    //event->getSS() << "hello sylar log";

    //logger->log(sylar::LogLevel::DBUG, event);

    SYLAR_LOG_INFO(logger) << "test macro";
    SYLAR_LOG_ERROR(logger) << "test macro error";

    SYLAR_LOG_FMT_ERROR(logger,"test macro format error %s", "aa");

    auto l = sylar::LoggerMgr::getInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";

    return 0;
}
