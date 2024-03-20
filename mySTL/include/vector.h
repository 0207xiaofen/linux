#ifndef __VECTOR_H__
#define __VECTOR_H__

#include"alloc.h"
#include"alloctor.h"
#include<cstring>
#include<utility>
#include<iterator>
#include"construct.h"

namespace wx{

template<class T, class Alloc = wx::simple_Allocator<T>>
class Vector{
private:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef value_type* iterator;
private:
    T *m_data;
    size_t m_size;          //当前元素个数
    size_t m_capacity;      //vector最大容量
    Alloc m_alloc;
private:
    // iterator start;   //第一个元素
    // iterator finish;     //最后一个元素末尾
    // iterator m_max_size;    //最大可用空间
public:
    iterator begin() noexcept{
        return m_data;
    }

    iterator end() noexcept{
        return m_data + m_size;
    }

public:
    Vector() noexcept{
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    explicit Vector(size_t n){      //explicit避免隐式构造
        m_data = m_alloc.allocate(n);
        m_size = n;
        m_capacity = n;
    }

    explicit Vector(size_t n, const T &val){
        //m_data = new T[n];
        m_data = m_alloc.allocate(n);
        for(size_t i=0; i<n; ++i){
            //m_data[i] = val;
            construct(&m_data[i], val);
        }
        m_size = n;
        m_capacity = n;
    }

    template<std::random_access_iterator iter = iterator>
    explicit Vector(iter first, iter last){
        size_t diff = last - first;
        if(diff == 0) return;
        m_size = diff;
        m_capacity = diff;
        //m_data = new T[diff];
        m_data = m_alloc.allocate(diff);
        for(size_t i=0; i<diff; ++i){
            //m_data[i] = *first;
            construct(&m_data[i], *first);
            ++first;
        }
    }

    Vector(std::initializer_list<T> list): Vector(list.begin(), list.end()){        //接收初始化列表作为参数

    }

    T const&operator[](size_t i) const noexcept{
        return m_data[i];
    }

    T &operator[](size_t i) noexcept{
        return m_data[i];
    }

    T const& at(size_t i) const{
        if(i >= m_size) [[unlikely]] throw std::out_of_range("Vector at");
        return m_data[i];
    }

    T &at(size_t i){
        if(i >= m_size) [[unlikely]] throw std::out_of_range("Vector at");
        return m_data[i];
    }

    T const&back() const noexcept{
        return m_data[m_size-1];
    }

    T &back() noexcept{
        return m_data[m_size-1];
    }

    T const&front() const noexcept{
        return *m_data;
    }

    T &front()  noexcept{
        return *m_data;
    }

    void push_back(const T& val){
        if(m_size == m_capacity){
            reserve(m_capacity * 2);
        }
        //m_data[m_size] = val;
        construct(&m_data[m_size], val);
        ++m_size;
    }

    void push(T &&val){         //用于不可拷贝的对象
        if(m_size == m_capacity){
            reserve(m_capacity * 2);
        }
        //m_data[m_size] = std::move(val);
        construct(&m_data[m_size], std::move(val));
        ++m_size;
    }

    void pop_back(){
        destroy(&m_data[m_size-1]);
        --m_size;
    }

    void resize(size_t n){
        // T *old_data = m_data;
        // size_t old_size = m_size;
        // if(n == 0){
        //     m_data = nullptr;
        //     m_size = 0;
        // }else{
        //     m_data = new T[n]{};
        //     m_size = n;
        // }
        // if(old_data){
        //     size_t copy_size = std::min(m_size, old_size);
        //     if(copy_size != 0){
        //         memcpy(m_data, old_data, copy_size * sizeof(T));
        //     }
        //     delete[] old_data;
        // }
        reserve(n);
        //m_size = n;
    }

    void reserve(size_t new_cap){       //扩容
        if(new_cap <= m_capacity)[[likely]] return;
        else{
            pointer old_data = m_data;
            m_capacity = std::max(new_cap, m_capacity * 2);
            //m_data = new T[m_capacity];
            m_data = m_alloc.allocate(m_capacity);
            if(m_size){
                //memcpy(m_data, old_data, m_size * sizeof(T));
                for(size_t i=0; i<m_size; ++i){
                    construct(&m_data[i], std::move_if_noexcept(old_data[i]));
                }
            }
            if(old_data){
                //delete[] old_data;
                for(size_t i=0; i<m_size;++i){
                    destroy(&old_data[i]);
                }
            }
        }
    }

    void shrink_to_fit(){       //降低容量
        pointer old_data = m_data;
        m_capacity = m_size;
        if(m_size == 0){
            m_data = nullptr;
        }else{
            //m_data = new T[m_capacity];
            m_data = m_alloc.allocate(m_capacity);
        }
        if(old_data){
            //memcpy(m_data, old_data, m_size * sizeof(T));
            for(size_t i=0; i<m_size; ++i){
                construct(&m_data[i], std::move_if_noexcept(old_data[i]));
                destroy(&old_data[i]);
            }
            //delete[] old_data;
        }
    }

    void clear() noexcept{
       resize(0);
    }

    size_t size() const noexcept{
        return m_size;
    }
    
    size_t &size() noexcept{
        return m_size;
    }

    size_t capacity() const noexcept{
        return m_capacity;
    }

    size_t &capacity() noexcept{
        return m_capacity;
    }

    pointer get() const noexcept{
        return m_data;
    }

    pointer &get() noexcept{
        return m_data;
    }

    ~Vector() noexcept{
        //delete[] m_data;
        for(size_t i=0; i<m_size; ++i){
            destroy(&m_data[i]);
        }
        m_size = 0;
    }

    Vector(const Vector& that){     //拷贝构造函数
        m_size = that.size();
        m_capacity = that.capacity();
        if(m_size != 0){
            //m_data = new T[m_capacity]{};
            m_data = m_alloc.allocate(m_capacity);
            for(size_t i=0; i<m_size; ++i){
                construct(&m_data[i], std::as_const(that.get()[i]));    //std::as_const --> T const&
            }
            //memcpy(m_data, that.get(), m_size * sizeof(T));
        }else{
            m_data = nullptr;
        }
    }

    Vector &operator=(const Vector& that){      //拷贝赋值函数
        clear();
        m_size = that.size();
        m_capacity = that.capacity();
        if(m_size != 0){
            // m_data = new T[m_capacity];
            // memcpy(m_data, that.get(), m_size * sizeof(T));
            m_data = m_alloc.allocate(m_capacity);
            for(size_t i=0; i<m_size; ++i){
                construct(&m_data[i], std::as_const(that.get()[i]));
            }
        }
        return *this;
    }

    Vector(Vector&& that) noexcept{      //移动构造函数
        // m_data = that.get();
        // m_size = that.size();
        // that.get() = nullptr;
        // that.size() = 0;
        m_data = std::exchange(that.get(), nullptr);
        m_size = std::exchange(that.size(), 0);
        m_capacity = std::exchange(that.capacity(), 0);
    }

    Vector &operator=(Vector&& that) noexcept{       //移动赋值函数
        if(this != &that)[[likely]]{
            pointer old_data = m_data;
            size_t old_size = m_size;
            m_data = std::exchange(that.get(), nullptr);
            m_size = std::exchange(that.size(), 0);
            m_capacity = std::exchange(that.capacity(), 0);
            if(old_data){
                //delete[] old_data;
                for(size_t i=0; i<m_size; ++i){
                    destroy(&old_data[i]);
                }
            }
        }
        return *this;
    }
public:
    // void erase(size_t i){
    //     if(i >=0 && i <= m_size-1){
    //         for(size_t j=i+1; j!=m_size; ++j){
    //             m_data[j-1] = std::move(m_data[j]);
    //         }
    //         resize(m_size-1);
    //         --m_size;
    //     }
    // }

    void erase(const iterator it){
        size_t i = it - m_data;
        for(size_t j=i+1; j<m_size; ++j){
            m_data[j-1] = std::move(m_data[j]);
        }
        resize(m_size-1);
        --m_size;
    }

    void erase(const iterator first, const iterator last){
        size_t i = first - m_data;
        size_t j = last - m_data;
        size_t diff = last - first;
        for(size_t k=i+1; k<m_size; ++k,++j){
            m_data[k-1] = std::move(m_data[j]);
        }
        m_size -= diff;
        resize(m_size);
    }

    bool empty() const noexcept{
        return m_size;
    }
public:
    void assign(size_t n, T val = 0){
        reserve(n);
        m_size = n;
        m_capacity = n;
        for(size_t i=0; i<n; ++i){
            //m_data[i] = val;
            construct(&m_data[i], val);
        }
    }

    template<std::random_access_iterator Iter = iterator>
    void assign(Iter first, Iter last){
        if(m_data){
            //delete[] m_data;
            for(size_t i=0; i<m_size; ++i){
                destroy(&m_data[i]);
            }
        }
        size_t diff = last - first;
        //m_data = new T[diff];
        m_data = m_alloc.allocate(diff);
        m_size = diff;
        m_capacity = diff;
        for(size_t i=0; i<diff; ++i){
            //m_data[i] = *first;
            construct(&m_data[i], *first);
            ++first;
        }
    }

    void assign(std::initializer_list<T> list){
        assign(list.begin(), list.end());
    }

    //void insert(size)

};


}


#endif