#include "project_includes.h"


double ReadNamedData(std::ifstream& fin)
{
	std::string s;
	double ipt;
	
	fin >> s >> ipt;
	
	return ipt;
}

matrix ReadMatrix(int n, int m, std::ifstream& fin)
{
	matrix M(n, vec(m));
	
	for(auto& row: M)
	{
		for(auto& x: row)
		{
			fin >> x;
		}
	}
	
	return M;
}

void FileOpenFailure(const std::string& file_name)
{
	std::cout << file_name << " file_open_failed and new created, restart program! \n";
	std::ofstream fout(file_name);
}
