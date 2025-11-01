#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <cerrno>
#include <cstdlib>
#include <vector>

struct ThreadData{
	std::vector<int>* arr;
	size_t start;
	size_t end;
	size_t sum;
};

void* sumArr(void* arg){
	ThreadData* th = (ThreadData*)arg;
	size_t sum = 0;
	for(size_t i = (*th).start; i < (*th).end; ++i){
		sum += (*(*th).arr)[i];
	}
	(*th).sum = sum;	
	return nullptr;
}	

void fillArr(std::vector<int>& v){
	for(size_t i = 0; i < v.size(); ++i){
		v[i] = std::rand()%100;
	}	
}

double sumArrWithThreads(std::vector<int>& v, const int M){
	size_t fSum = 0;
	size_t localSize = v.size() / M;
	std::vector<pthread_t> threads(M);
	std::vector<ThreadData> data(M);
	

	for(int i = 0; i < M; ++i){
		ThreadData tmp;	
		tmp.start = localSize*i;
		tmp.end = localSize*(i+1);
		tmp.arr = &v;

		data[i] = tmp;	
	}
	
	data.back().end += v.size() - localSize*M;
	
	bool checkTh = false;

	for(int i = 0; i < M; ++i){
		int check = pthread_create(&threads[i], nullptr, sumArr, &data[i]);
		if(check != 0){
			std::cerr<<"Failed to create thread"<<std::endl;
			checkTh = true;
		}
				
	}


	for(int i = 0; i < M; ++i){
		int check = pthread_join(threads[i], nullptr);
		if(check != 0){
			std::cerr<<"Failed to join thread"<<std::endl;
			checkTh = true;
		}

		fSum += data[i].sum;	
		
	}

	if(checkTh){
		exit(1);
	}	

	return fSum;	
}	

int main(int argc, char** argv){
	if(argc != 3 || std::atoi(argv[1]) < 1000000 || std::atoi(argv[2]) < 0){
		std::cerr<<"Invalid input"<<std::endl;
		exit(1);
	}

	size_t N = std::atoi(argv[1]);
	int M = std::atoi(argv[2]);
	
	std::vector<int> v(N);
	fillArr(v);
       	
	struct timeval time;
	ThreadData th;
	th.arr = &v;
	th.start = 0;
	th.end = v.size();

///////////////////////////////////////////////////////////////////
	
	gettimeofday(&time, nullptr);
	double before = time.tv_sec + time.tv_usec / 1e6;
	
	sumArr(&th);
	size_t sum1 = th.sum;

	gettimeofday(&time, nullptr);
	double after = time.tv_sec + time.tv_usec / 1e6;

	double time1 = after - before;
	std::cout<<"sum "<< sum1 << ", spent without threads: "<<time1<<std::endl;
	
///////////////////////////////////////////////////////////////////
	
	gettimeofday(&time, nullptr);
	before = time.tv_sec + time.tv_usec / 1e6;	

	size_t sum2 = sumArrWithThreads(v, M);
	
	gettimeofday(&time, nullptr);
	after = time.tv_sec + time.tv_usec / 1e6;

	double time2 = after - before;
	std::cout<<"sum "<< sum2 << ", spent with threads "<<time2<<std::endl;


	return 0;
}
