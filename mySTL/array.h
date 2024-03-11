#ifndef __ARRAY_H__
#define __ARRAY_H__

#include<string>

#if defined(_MSC_VER)
#define _LIBWXCXX_UNREACHABLE() __assume(0)
#elif defined(__clang__)
#define _LIBWXCXX_UNREACHABLE() __builtin_unreachable()
#elif defined(__GNUC__)
#define _LIBWXCXX_UNREACHABLE() __builtin_unreachable()
#else
#define _LIBWXCXX_UNREACHABLE() do {} while (1)
#endif

namespace wx{

template<class T, size_t N>
struct Array{
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;

    T array[N];

    T &operator[](size_t i){
        return array[i];
    }

    T const &operator[](size_t i) const{
        return array[i];
    }

    T &at(size_t i){
        if(i >= N) [[unlikely]] {
            throw std::runtime_error("out of range! i=" + std::to_string(i));
        }
        return array[i];
    }

    T const &at(size_t i) const{
        if(i >= N) [[unlikely]] {
            throw std::runtime_error("out of range! i=" + std::to_string(i));
        }
        return array[i];
    }

    static constexpr size_t size(){     //N是编译器常量
        return N;
    }

    iterator begin(){
        return array;
    }

    iterator end(){
        return array + N;
    }

    reference front(){
        return *array;
    }

    const_reference front() const{
        return array[0];
    }

    reference back(){
        return *(array + N);
    }

    const_reference back() const{
        return array[N-1];
    }

    pointer data() noexcept{
        return array;
    }

    const_pointer data() const noexcept{
        return array;
    }

    constexpr bool empty() const noexcept{
        return false;
    }

    constexpr size_t max_size() const noexcept{
        return N;
    }

    void fill(const T& val){
        for(int i=0; i<N; ++i){
            array[i] = val;
        }
    }

    void swap(Array<T,N>& a) noexcept{
        Array<T,N> tmp = *this;
        *this = a;
        a = tmp;
    }
};


template<class T>
struct Array<T,0>{
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;

    T &operator[](size_t i){
        _LIBWXCXX_UNREACHABLE();
    }

    T const &operator[](size_t i) const{
        _LIBWXCXX_UNREACHABLE();
    }

    T &at(size_t i){
        _LIBWXCXX_UNREACHABLE();
    }

    T const &at(size_t i) const{
        _LIBWXCXX_UNREACHABLE();
    }

    static constexpr size_t size(){     //N是编译器常量
        return 0;
    }

    iterator begin(){
        return nullptr;
    }

    iterator end(){
        return nullptr;
    }

    reference front(){
        _LIBWXCXX_UNREACHABLE();
    }

    const_reference front() const{
        _LIBWXCXX_UNREACHABLE();
    }

    reference back(){
        _LIBWXCXX_UNREACHABLE();
    }

    const_reference back() const{
        _LIBWXCXX_UNREACHABLE();
    }

    pointer data() noexcept{
        return nullptr;
    }

    const_pointer data() const noexcept{
        return nullptr;
    }

    constexpr bool empty() const noexcept{
        return true;
    }

    constexpr size_t max_size() const noexcept{
        return 0;
    }

    void fill(const T& val){
    }

    void swap(Array<T,0>& a) noexcept{
    }
};

template<class T, class ...Ts>
Array(T, Ts...)->Array<T, sizeof...(Ts)+1>;

}


#endif