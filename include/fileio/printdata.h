#ifndef PRINTDATA_H
#define PRINTDATA_H

#include "decs/simtypedefs.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class PrintData
{
	
public:
	static void print_matrix(const std::string& s, const Tfloat& matrix);
	static void print_matrix(const std::string& s, const std::vector<float>& v, const int n, const int m);
};
#endif
