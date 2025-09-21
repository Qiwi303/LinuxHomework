#ifndef CNAS_CLASS_H
#define CNAS_CLASS_H
#include <iostream>
#include <cmath>
class ComplexNumbers{
public:
	ComplexNumbers(const double _re, const double _im): re(_re), im(_im){}
	ComplexNumbers(): re(0), im(0){}
		
	ComplexNumbers operator+(const ComplexNumbers &other);
	
	ComplexNumbers operator-(const ComplexNumbers &other);

	ComplexNumbers operator*(const ComplexNumbers &other);
	
	ComplexNumbers operator*(const double num);

	double getAbsoluteValue() const {
		return sqrt(re*re + im*im);
	}

	bool operator>(const ComplexNumbers& other){
		return this->getAbsoluteValue() > other.getAbsoluteValue(); 
	}

	friend std::ostream& operator<<(std::ostream& os, const ComplexNumbers& other){
		os <<"("<< other.re<<"; "<<other.im<<")";
		return os;
	}	

protected:
	double re, im;
};
#endif
