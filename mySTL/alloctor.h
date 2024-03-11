#ifndef __ALLOCTOR_H__
#define __ALLOCTOR_H__

#include"construct.h"
#include"alloc.h"

namespace wx{

template<class T, class Alloc = alloc>
class simple_Allocator{
public:
    typedef T value_type;

public:
    T* allocate(size_t n){
        return (T*)Alloc::allocate(n * sizeof(T));
    }

    T* allocate(){
        return (T*)Alloc::allocate(sizeof(T));
    }

    void deallocate(void* p, size_t n){
        Alloc::deallocate(p, n * sizeof(T));
    }
    void deallocate(void* p){
        Alloc::deallocate(p);
    }
};


}


#endif