#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__
//获取对象型别的特征，拷贝，构造，析构函数是否为默认
//只要用于constructor.h中destroy析构对象，判断对象析构函数是否无关痛痒

namespace wx{

struct true_type{};     //是trivial destructor，用的默认析构函数
struct false_type{};    //不是trivial destructor，有自定义析构函数

template<class T>
class type_traits{
    typedef false_type is_trivially_destructible;
    typedef false_type is_trivially_copy_constructor;
    typedef false_type is_trivially_default_constructor;
};

template<class T>
class type_traits<T*>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<char>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<int>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<short>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<long>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<long long>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<float>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<unsigned int>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<unsigned short>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<unsigned long>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

template<>
class type_traits<unsigned long long>{
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_copy_constructor;
    typedef true_type is_trivially_default_constructor;
};

    
}


#endif