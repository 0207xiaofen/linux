#include<iostream>
#include"vector.h"

int main(){
    wx::Vector<int> vec;
    std::cout << "m_capacity = " << vec.capacity() << 
            " m_size = " << vec.size() << std::endl;
    vec.resize(4);
    std::cout << "m_capacity = " << vec.capacity() << 
            " m_size = " << vec.size() << std::endl;
    for(int i=0; i<vec.capacity(); ++i){
        std::cout << "vec.push_back(" << i << std::endl;
        vec.push_back(i);
    }
    vec.push_back(4);
    for(auto &it:vec){
        std::cout << "vec[" << it << "]" << std::endl;
    }
    std::cout << "m_capacity = " << vec.capacity() << 
            " m_size = " << vec.size() << std::endl;
    auto it = vec.begin() + 2;
    vec.erase(vec.begin(),it);
    for(auto &it:vec){
        std::cout << "vec[" << it << "]" << std::endl;
    }
    std::cout << "m_capacity = " << vec.capacity() << 
            " m_size = " << vec.size() << std::endl;
    std::cout << "---------------------------" << std::endl;
    auto a = vec;
    for(int i=0;i<a.size();++i){
        std::cout << "a[" << i << "]=" << a[i] << std::endl; 
    }
    std::cout << "a.size = " << a.size() << " a.capacity=" << a.capacity() << std::endl;
    std::cout << "---------------------------" << std::endl;
    auto b(std::move(a));
    for(int i=0;i<b.size();++i){
        std::cout << "b[" << i << "]=" << b[i] << std::endl; 
    }
    std::cout << "b.size = " << b.size() << " b.capacity=" << b.capacity() << std::endl;
    wx::Vector<int> la;
    la.assign(6,6);
    for(int i=0;i<la.size();++i){
        std::cout << "la[" << i << "]=" << la[i] << std::endl; 
    }
    std::cout << "la.size = " << la.size() << " la.capacity=" << la.capacity() << std::endl;
    std::cout << "---------------------" << std::endl;
    int first[4] = {24,24,24,24};
    la.assign(first, first+4);
    for(int i=0;i<la.size();++i){
        std::cout << "la[" << i << "]=" << la[i] << std::endl; 
    }
    std::cout << "la.size = " << la.size() << " la.capacity=" << la.capacity() << std::endl;
    std::cout << "---------------------" << std::endl;
    wx::Vector<int> ans(first, first+4);
    for(int i=0;i<ans.size();++i){
        std::cout << "ans[" << i << "]=" << ans[i] << std::endl; 
    }
    std::cout << "ans.size = " << ans.size() << " ans.capacity=" << ans.capacity() << std::endl;
    std::cout << "---------------------" << std::endl;

    wx::Vector<int> llk({99,99,99,9,1});
    for(int i=0;i<llk.size();++i){
        std::cout << "llk[" << i << "]=" << llk[i] << std::endl; 
    }
    std::cout << "llk.size = " << llk.size() << " llk.capacity=" << llk.capacity() << std::endl;
    std::cout << "---------------------" << std::endl;
    llk.pop_back();
    for(int i=0;i<llk.size();++i){
        std::cout << "llk[" << i << "]=" << llk[i] << std::endl; 
    }
    std::cout << "llk.size = " << llk.size() << " llk.capacity=" << llk.capacity() << std::endl;
    return 0;
}