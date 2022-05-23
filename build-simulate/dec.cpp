#include "dec.h"


double ReadNamedData(std::ifstream& fin)
{
	std::string s;
	double ipt;
	
	fin >> s >> ipt;
	
	return ipt;
}
/*
void FileOpenFailure(const std::string& file_name)
{
	std::cout << file_name << " file_open_failed and new created, restart program! \n";
	std::ofstream fout(file_name);
}
*/
