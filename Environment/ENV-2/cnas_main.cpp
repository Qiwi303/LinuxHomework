#include <iostream>
#include "cnas_class.h"
#include "sorting_algorithm.h"
#include <vector>

template<typename T>
void print(std::vector<T> vec){
	for(int i = 0; i < (int)vec.size(); ++i){
		std::cout<<vec[i]<<" ";
	}
	std::cout<<std::endl;
}


int main(){
	ComplexNumbers num1(2, 3);
	ComplexNumbers num2(4, 5);
	
	std::cout<<num1<<" + "<<num2<<" = "<<num1+num2<<std::endl;
	std::cout<<num1<<" - "<<num2<<" = "<<num1-num2<<std::endl;
	std::cout<<num1<<" * "<<num2<<" = "<<num1*num2<<std::endl;
	std::cout<<num1<<" * 3.5"<<" = "<<num1*3.5<<std::endl;

	std::vector<ComplexNumbers> vec;
	vec.push_back(ComplexNumbers(3, 4));
	vec.push_back(ComplexNumbers(1, 1));
	vec.push_back(ComplexNumbers(0, 2));
	vec.push_back(ComplexNumbers(5, 12));
	vec.push_back(ComplexNumbers(2, -2));
	
	std::cout<<"unsorted vector: ";
	print<ComplexNumbers>(vec);
	bubbleSort(vec);
	std::cout<<"sorted vector: ";
	print<ComplexNumbers>(vec);

	return 0;
}

