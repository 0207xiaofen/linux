#include"algorithm.h"
#include"vector.h"
#include<iostream>

int main(){
	wx::Vector<int> vec({99,23,89,19,66,12,0,33});
	auto it = wx::find(vec.begin(),vec.end(), 100);
	if(it == vec.end()){
		std::cout << "can't find it in vec!!!" << std::endl;
	}else 
		std::cout << "0 is in vec[" << it-vec.begin() << "]" << std::endl;
	return 0;
}
