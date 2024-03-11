#include "alloctor.h"
#include<iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
    std::cout << "allocator test" << std::endl;
    vector<int, wx::simple_Allocator<int> > v;
    v.push_back(1);
    v.push_back(2);
    for(int i=0;i<20;++i){
        v.push_back(i);
    }
    for (int i = 0; i < v.size(); ++i)
    {
        cout << v[i] << endl;
    }

    return 0;
}