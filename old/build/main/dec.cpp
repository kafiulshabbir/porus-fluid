#include "dec.h"

void PrintMatrix(const matrix& v, const std::string& file_name)
{
	std::ofstream fout(file_name);
	
	for(const auto& row: v)
	{
		for(const auto& x: row)
		{
			fout << x << ' ';
		}
		fout << '\n';
	}
}
