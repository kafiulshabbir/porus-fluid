#ifndef CMDIOPRINT_H
#define CMDIOPRINT_H

#include "decl/decltypedef.h"
#include "fileio/fileioread.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace cmdio
{
	class Print
	{
		
	public:
		// print matrix := pmat
		static void pmat(const std::string& s, const Tdouble& matrix);
		static void pmat(const std::string& s, const TMns& mnsc); // compressed to a double
		static void pmat(const std::string& s,
			const std::vector<double>& v, const int n, const int m); //linear pressure vector printed as a matrix
		
		static void pmnsc(const TMns& matrix); // matric values are printed in a new line
		static void diamension(); // read and print the diamension of each file
	};
}
#endif
