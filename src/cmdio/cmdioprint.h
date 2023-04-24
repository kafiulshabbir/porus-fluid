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
		static void pmat(const std::string& s, const Tfloat& matrix);
		static void pmat(const std::string& s, const TMns& mnsc); // compressed to a float
		static void pmat(const std::string& s,
			const std::vector<float>& v, const int n, const int m); //linear pressure vector printed as a matrix
		
		static void pmnsc(const TMns& matrix); // matric values are printed in a new line
		static void padd(const std::string& s,
			const std::vector<std::vector<std::vector<float>>>& matrix); // whatever is required to add to each tube is printed with coordinates
		static void diamension(); // read and print the diamension of each file
	};
}
#endif
