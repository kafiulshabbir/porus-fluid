#include "fileio/fileread.h"
#include "fileio/printdata.h"
#include <iostream>

int main()
{
	while(true)
	{
		const auto input_data = fileread::Func::loop_until_proper_files();
		
		PrintData::print_matrix("radius", input_data.radius);
		
		std::cout << "--------------------\n\n\n" << std::endl;
		std::cin.get();
	}
	return 0;
}
