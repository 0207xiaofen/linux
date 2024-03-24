#include"../include/list.h"
#include<iostream>

int main(){
    wx::LIST<int> l;
    l.push_back(5);
    for(int i=0;i<9;++i){
        l.push_back(i);
    }
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    l.pop_back();
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    l.pop_front();
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    for(int i=66; i<70; ++i){
        l.push_front(i);
    }
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    // std::cout << "l is empty ?  " << bool(l.empty()) << std::endl;
    // std::cout << "-----------------------" << std::endl;
    // std::cout << "l.back = " << l.back() << std::endl;
    // std::cout << "l.front = " << l.front() << std::endl;
    l.emplace(l.begin(), 100001);
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    l.erase(l.begin());
    l.erase(l.begin(), l.begin()+3);
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    l.resize(5);
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    l.resize(10);
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    int a[] = {44,55,66,77,88};
    l.insert(l.end(), a ,a+5);
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    l.insert(l.begin(), {1001,1002,1003,1004,1005,1006});
    for(auto &i:l){
        std::cout << i << std::endl;
    }
    std::cout << l.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    wx::LIST<int> k(8,8);
    for(auto &i:k){
        std::cout << i << std::endl;
    }
    std::cout << k.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    wx::LIST<int> ll(a, a+5);
    for(auto &i:ll){
        std::cout << i << std::endl;
    }
    std::cout << "ll.size = " << ll.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    wx::LIST<int> kkl({12,14,16,18,20,22,24});
    for(auto &i:kkl){
        std::cout << i << std::endl;
    }
    std::cout << kkl.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    wx::LIST<int> copy(k);
    for(auto &i:copy){
        std::cout << i << std::endl;
    }
    std::cout << "copy.size = " << copy.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    copy = kkl;
    for(auto &i:copy){
        std::cout << i << std::endl;
    }
    std::cout << "copy.size = " << copy.size() << std::endl;
    std::cout << "----------------------" << std::endl;
    wx::LIST<int> move(std::move(ll));
    for(auto&i:move){
        std::cout << i << std::endl;
    }
    std::cout << "move.size = " << move.size() << std::endl;
    std::cout << "----------------------" << std::endl;
    for(auto &i:ll){
        std::cout << i << std::endl;
    }
    std::cout << "ll.size = " <<  ll.size() << std::endl;
    std::cout << "---------------------" << std::endl;

    ll = {909,908,808,807};
    for(auto &i:ll){
        std::cout << i << std::endl;
    }
    std::cout << "ll.size = " <<  ll.size() << std::endl;
    std::cout << "---------------------" << std::endl;
    copy = {123,234,345,456,567,678,789,890};
    for(auto &i:copy){
        std::cout << i << std::endl;
    }
    std::cout << "copy.size = " << copy.size() << std::endl;
    std::cout << "----------------------" << std::endl;

    copy.assign(6,6);
    for(auto &i:copy){
        std::cout << i << std::endl;
    }
    std::cout << "copy.size = " << copy.size() << std::endl;
    std::cout << "----------------------" << std::endl;

    copy.assign({44,33,3443,4433,4334,5678});
    for(auto &i:copy){
        std::cout << i << std::endl;
    }
    std::cout << "copy.size = " << copy.size() << std::endl;
    std::cout << "----------------------" << std::endl;
    return 0;
}