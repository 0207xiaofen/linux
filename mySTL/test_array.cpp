#include<iostream>
#include"array.h"

void PrintArray(wx::Array<int, 5>const &a){
    for(int i=0;i<a.size();++i){
        
        std::cout << a.at(i) << std::endl;
    }
}

void iterPrint(wx::Array<int, 8> &a){
    for(wx::Array<int, 32>::iterator it = a.begin(); it != a.end(); ++it){
        std::cout << a.front() << std::endl;
    }
}

void pointerPrint(const int* a, size_t n){
    for(int i=0; i<n; ++i){
        std::cout << *(a + i) << std::endl;
    }
}

int main(){
    auto h = wx::Array{1,2,3,4,5};
    PrintArray(h);
    // wx::Array<int, 8> a{1,2,3,4,5,6,7,8};
    // //PrintArray(a);
    // //iterPrint(a);
    // // for(auto &v:a){
    // //     std::cout << v << std::endl;
    // // }
    // pointerPrint(a.data(), a.size());
    // std::cout << "array empty test:" << a.empty() << std::endl;
    // a.fill(25);
    // PrintArray(a);
    // wx::Array<int, 8> b{8,7,6,5,4,3,2,1};
    // a.swap(b);
    // PrintArray(a);
    // std::cout << "________________________________" << std::endl;
    // PrintArray(b);

    // wx::Array<int, 0> c;
    // std::cout<<c.at(1);
    //std::cout << "_" << std::endl;
}