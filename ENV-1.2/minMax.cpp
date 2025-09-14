#include<iostream>
#include<string>
int main(int argc, char* argv[]){
	int min = std::stoi(argv[1]);
	int max = std::stoi(argv[1]);

	for(int i = 2; i < argc; ++i){
		size_t pos;
		std::string arr = argv[i];
		int x = std::stoi(argv[i], &pos);
		if(pos != arr.size()) {
			std::cout<<"invalid input"<<std::endl;
			return 1;
		}
		if(x < min) min = x;
		if(x > max) max = x;
	}

	std::cout<<min<<" "<<max<<std::endl;
	return 0;
}
