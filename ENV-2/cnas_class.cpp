#include "cnas_class.h"


ComplexNumbers ComplexNumbers::operator+(const ComplexNumbers &other){
	ComplexNumbers tmp;
	tmp.re = this->re + other.re;
	tmp.im = this->im + other.im;
	return tmp;
}


ComplexNumbers ComplexNumbers::operator-(const ComplexNumbers &other){
	ComplexNumbers tmp;
	tmp.re = this->re - other.re;
	tmp.im = this->im - other.im;
	return tmp;
}


ComplexNumbers ComplexNumbers::operator*(const ComplexNumbers &other){
	ComplexNumbers tmp;
	tmp.re = this->re*other.re - this->im*other.im;
	tmp.im = this->re*other.im + this->im*other.re;
	return tmp;
}


ComplexNumbers ComplexNumbers::operator*(const double num){
	ComplexNumbers tmp;
	tmp.re = this->re * num;
       	tmp.im = this->im * num;
	return tmp;
}	
