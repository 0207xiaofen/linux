#include"sylar.h"
#include"iomanager.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int sockfd = 0;

void test_fiber(){
    SYLAR_LOG_INFO(g_logger) << "test_fiber";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("39.156.66.10");

    if(!(connect(sockfd, (sockaddr*)&addr, sizeof(addr)))){

    }else if(errno == EINPROGRESS){
        SYLAR_LOG_INFO(g_logger) << "add event errno=" << errno << " " << strerror(errno);
        sylar::IOManager::GetThis()->addEvent(sockfd, sylar::IOManager::READ, [](){
            SYLAR_LOG_INFO(g_logger) << "read callback";
        });
        sylar::IOManager::GetThis()->addEvent(sockfd, sylar::IOManager::WRITE, [](){
            SYLAR_LOG_INFO(g_logger) << "write callback";
            sylar::IOManager::GetThis()->cancleEvent(sockfd, sylar::IOManager::READ);
            close(sockfd);
        });
    }else{
        SYLAR_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
    }

}

void test1(){
    sylar::IOManager iom(2, false);
    iom.schedule(&test_fiber);

}

sylar::Timer::ptr s_timer;
void test_timer(){
    sylar::IOManager iom(2);
    s_timer = iom.addTimer(1000, [](){
        static int i = 0;
        SYLAR_LOG_INFO(g_logger) << "hello timer i=" << i;
        if(++i == 3){
            //s_timer->cancel();
            s_timer->reset(2000,true);
        }
    }, true);
}

int main(int argc, char** argv){
    test_timer();
    //test1();
    return 0;
}