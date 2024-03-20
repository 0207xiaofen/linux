// #include<iostream>
// #include"unique_ptr.h"

// void func(wx::Uniquer_ptr<int> const &a){
    
// }

// void func2(wx::Uniquer_ptr<int> a){

// }

// struct Animal{
//     int a, b ,c;
// };

// int main(){
//     auto ll = wx::makeUnique<int>();    //相当于new int(); 初始化为0
//     std::cout << "____________________" << std::endl;
//     std::cout << *ll << std::endl;
//     auto lk = wx::makeUnique_forOverwrite<int>();
//     std::cout << *lk << std::endl;
//     auto m = wx::makeUnique<Animal>(1,2,3);
//     auto x = wx::Uniquer_ptr<Animal>(new Animal{1,2,3});
//     std::cout << x->b << std::endl;
//     std::cout << m->b << std::endl;
//     int i;
//     auto a = wx::Uniquer_ptr<int>(new int(i));
//     auto b = std::move(a);
//     func(a);
    
// }

#include <cstdio>
#include <vector>
#include <iostream>
#include<memory>
#include "unique_ptr.h"

struct MyClass {
    int a, b, c;
};

struct Animal {
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

struct Dog : Animal {
    int age;

     Dog(int age_) : age(age_) {
     }

    virtual void speak() {
        printf("Bark! ");
    }
};

struct Cat : Animal {
    int &age;

    Cat(int &age_) : age(age_) {
    }

    virtual void speak() {
        printf("Meow!");
    }
};

int main() {
    std::vector<wx::Uniquer_ptr<Animal>> zoo;
    int age = 3;
    zoo.push_back(wx::makeUnique<Cat>(age));
    zoo.push_back(wx::makeUnique<Dog>(age));
    for (auto const &a: zoo) {
        a->speak();
    }
    //age++;
    for (auto const &a: zoo) {
        a->speak();
    }
    // std::vector<std::shared_ptr<Animal>> zoo;
    // int age = 3;
    // zoo.push_back(std::make_shared<Cat>(age));
    // zoo.push_back(std::make_shared<Dog>(age));
    // for(auto const&a: zoo){
    //     a->speak();
    // }
    return 0;
}