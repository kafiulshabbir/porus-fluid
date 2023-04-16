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
		static void pmat(const std::string& s, const Tfloat& matrix);
		static void pmat(const std::string& s, const TMns& mnsc);
		static void pmat(const std::string& s, const std::vector<float>& v, const int n, const int m);
		static void diamension();
	};
}
#endif
