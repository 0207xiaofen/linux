#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__
/*SGI将new分为两步，operate new分配内存，placement new构建对象
construct构建对象，destroy析构对象*/

#include<new> //使用placement new 必须包含该头文件
#include"type_traits.h" //用于判断迭代器所指对象型别
#include<iterator>

namespace wx{

template<class T1, class T2>
inline void construct(T1 *p, const T2& value){
    new (p) T1(value);  //placement new 在p这个地址上构建值为value，类型为T1的对象
}

template<class T>
inline void construct(T *p){
    new (p) T();  //调用默认构造函数
}

template<class T>
inline void  destroy(T *pointer){
    pointer->~T();  //接受一个指针，直接调用析构函数
}

template<class Iterator>
inline void destroy(Iterator first, Iterator last){
    _destroy(first, last, value_type(first));   //value_type获取迭代器first所指对象的型别
}

template<class Iterator, class T>
inline void _destroy(Iterator first, Iterator last, T*){
    typedef typename wx::type_traits<T> type_trivial();  //暂未实现萃取剂，暂时利用标准库的api
    _destroy_aux(first, last, type_trivial());
}

template<class Iterator>
inline void _destroy_aux(Iterator first, Iterator last, std::true_type) {} //标准库true_type表示该对象调用的默认析构函数，是trivial_destructor

template<class Iterator>
inline void _destroy_aux(Iterator first, Iterator last, std::false_type){  //false_type 表示该对象有自定义析构函数，需要调用析构函数
    for(; first < last; ++first){
        destroy(&*first);
    }
}



}


#endif