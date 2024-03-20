#ifndef __UNIQUE_PTR_H__
#define __UNIQUE_PTR_H__

#include<utility>

namespace wx{

template<class T, class U>
T exchange(T &dst, U &&new_value){
    T tmp = std::move(dst);
    dst = std::forward<U>(new_value);
    return tmp;
}

template<class T>
struct defaultDeleter{
    void operator()(T *p) const{
        delete p;
    }
};

template<class T>
struct defaultDeleter<T[]>{
    void operator()(T* p) const{
        delete[] p;
    }
};

template<class T, class Deleter = defaultDeleter<T>>
class Uniquer_ptr{
private:
    T *m_p;
    template<class U, class Udeleter>
    friend struct Uniquer_ptr;
public:
    Uniquer_ptr(){
        m_p = nullptr;
    }    

    explicit Uniquer_ptr(T *p){     //定义为显示构造，防止栈上的数据隐式转换
        m_p = p; 
    }

    Uniquer_ptr(Uniquer_ptr const&) = delete;     //禁用拷贝构造
    T &operator=(Uniquer_ptr const&) = delete;    //禁用拷贝赋值

    Uniquer_ptr(Uniquer_ptr && other){
        // m_p = other.m_p;
        // other.m_p = nullptr;
        m_p = exchange(other.m_p, nullptr);
    }

    T &operator=(Uniquer_ptr && other){
        if(this != &other) [[likely]] {
            if(m_p){
                Deleter{}(m_p);
                m_p = nullptr;
            }
            m_p = exchange(other.m_p, nullptr);
            return *this;
        }
    }

    template<class U, class Udeleter, class  = std::enable_if_t<std::is_convertible_v<U*, T*>>>  //多态指针转换
    Uniquer_ptr(Uniquer_ptr<U,Udeleter> &&dst){
        m_p = exchange(dst.m_p, nullptr);
    }

    T* operator->() const{
        return m_p;
    }

    T &operator*() const{
        return *m_p;
    }

    ~Uniquer_ptr(){
        if(m_p){
            Deleter{}(m_p);
        }
        //std::cout << "~uniquer_ptr()" << std::endl;
    } 

    T *get() const{
        return m_p;
    }

    T *release(){
        return exchange(m_p, nullptr);
    }

    T *reset(T *p = nullptr){
        if(m_p){
            Deleter{}(m_p);
        }
        m_p = p;
    }
};


template<class T, class Deleter>        //偏特化不能带有默认模板参数 like--->  class Deleter = defauleDeleter<T>
struct Uniquer_ptr<T[], Deleter> :public Uniquer_ptr<T, Deleter> {};

template<class T, class ...Args>
Uniquer_ptr<T> makeUnique(Args &&...args){
    return Uniquer_ptr<T>(new T(std::forward<Args>(args)...));      //c++20特性，当struct没有默认构造函数时，c++11用{}初始化，而c++20支持（）初始化
}

template<class T>
Uniquer_ptr<T> makeUnique_forOverwrite(){
    return Uniquer_ptr<T>(new T);
}

}

#endif