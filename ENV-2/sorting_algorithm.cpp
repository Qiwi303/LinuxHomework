#include "sorting_algorithm.h"
void bubbleSort(std::vector<ComplexNumbers>& vec){
        for(int i = 0; i < (int)vec.size() - 1; ++i){
		for(int j = 0; j < (int)vec.size() - i - 1; ++j){
	 		if(vec[j].getAbsoluteValue() > vec[j+1].getAbsoluteValue()){
				ComplexNumbers tmp = vec[j+1];
				vec[j+1] = vec[j];
				vec[j] = tmp;	
				}
		}
	}
	return;
}
