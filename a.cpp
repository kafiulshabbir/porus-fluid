#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
#include <limits>

#include "drw.h"

class A
{
	int a;
	int b;
	std::vector<int> c;
	double d;
};


int main()
{
	std::cout << sizeof(int) << std::endl;
	std::cout << sizeof(int*) << std::endl;
	std::cout << sizeof(float) << std::endl;
	std::cout << sizeof(float*) << std::endl;
	std::cout << sizeof(double) << std::endl;
	std::cout << sizeof(double*) << std::endl;
	std::cout << sizeof(A) << std::endl;
	std::cout << sizeof(A*) << std::endl;
	std::cout << sizeof(std::vector<int>) << std::endl;
	std::cout << sizeof(std::vector<int>*) << std::endl;
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(char*) << std::endl;
	std::cout << sizeof(short) << std::endl;
	std::cout << sizeof(short*) << std::endl;
	std::cout << sizeof(long double) << std::endl;
	std::cout << sizeof(long double*) << std::endl;
	
	return 0;
}
