//单例模式封装
#ifndef __SYLAR_SINGLETON_H__
#define __SYLAR_SINGLETON_H__

#include<memory>

namespace sylar{

template<class T, class X = void, int N = 0>
class Singleton{
public:
    static T* getInstance(){
        static T v;
        return &v;
    }
}; 


template<class T, class X = void, int N = 0>
class Singletonptr{
public:
    static std::shared_ptr<T> getInstance() {
        static std::shared_ptr<T> v(new T);
        return v;
    }
};


}



#endif