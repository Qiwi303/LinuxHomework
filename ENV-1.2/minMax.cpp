#include<iostream>
#include<string>
int main(int argc, char* argv[]){
	int min = std::stoi(argv[1]);
	int max = std::stoi(argv[1]);

	for(int i = 2; i < argc; ++i){
		int x = std::stoi(argv[i]);
		if(x < min) min = x;
		if(x > max) max = x;
	}

	std::cout<<min<<" "<<max<<std::endl;
	return 0;
}
