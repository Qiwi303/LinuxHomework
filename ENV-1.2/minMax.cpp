#include<iostream>

int main(int argc, char* argv[]){

	int min = *(argv[1]) - '0';
	int max = *(argv[1]) - '0';

	for(int i = 2; i < argc; ++i){
		int x = *(argv[i]) - '0';
		if(x < min) min = x;
		if(x > max) max = x;
	}

	std::cout<<min<<" "<<max<<std::endl;
	return 0;
}
