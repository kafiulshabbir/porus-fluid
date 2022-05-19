#include "type_fill.h"

namespace frw
{
	type_fill::type_fill(const std::string& name_file, int n): name_file(name_file), n(n) {}

	bool type_fill::read_file()
	{
		std::ifstream fin(name_file);
		
		if(!fin.is_open())
		{
			FileOpenFailure(name_file);
			return true;
		}
		
		ver = ReadMatrix(n - 1, n, fin);
		hor = ReadMatrix(n, n - 1, fin);
		
		std::cout << name_file << " correctly read!\n";
		return false;
	}
}

