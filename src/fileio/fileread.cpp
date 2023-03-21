#include "fileio/fileread.h"

template<class T>
static std::vector<std::vector<T>> table_from_linear(const std::vector<T>& lineardata, const int rows, const int cols)
{
	std::vector<std::vector<T>> table(rows, std::vector<T>(cols));
	int count = 0;
	for(auto& row: table)
	{
		for(auto& cell: row)
		{
			cell = lineardata[count ++];
		}
	}
	
	return table;
}

template<class T>
static std::pair<std::vector<std::vector<T>>, bool> read_file(const std::string& file_name)
{
	std::ifstream fin(file_name);
	int rows, cols;
	fin >> rows >> cols;
	
	std::vector<T> lineardata;
	T ipt;
	while(fin >> ipt)
	{
		lineardata.push_back(ipt);
	}
	
	std::pair<std::vector<std::vector<T>>, bool> outdata;
	outdata.second = false;
	
	if(lineardata.size() != rows * cols)
	{
		std::cout << "ERR-in file " << file_name << " rows, cols are declared differently than the actual data in them." << std::endl;
		return outdata;
	}
	
	outdata.first = table_from_linear(lineardata, rows, cols);
	outdata.second = true;
	
	return outdata;
}
	
std::pair<Tfloat, bool> fileread::Func::read_radius()
{
	return read_file<float>(filenames::FILE_RADIUS);
}

std::pair<Tmns, bool> fileread::Func::read_mnsc()
{
	return read_file<mns>(filenames::FILE_MNSC);
}

fileread::Data fileread::Func::read_all_input_files()
{
	Data data;
	data.success = false;
	
	const auto radius = read_radius();
	if(!radius.second)
	{
		std::cout << "ERR-radius.txt is corrupted" << std::endl;
		return data;
	}
	data.radius = radius.first;
	
	const auto mnsc = read_mnsc();
	if(!mnsc.second)
	{
		std::cout << "ERR-mnsc.txt meniscus configuration file is corrupted" << std::endl;
		return data;
	}
	data.mnsc = mnsc.first;
	
	const Diamension dradius(data.radius);
	const Diamension dmnsc(data.mnsc);
	if(!(dradius == dmnsc))
	{
		std::cout << "ERR-Diamensions of radius.txt and mnsc.txt do not match" << std::endl;
		return data;
	}
	
	data.diamension = dradius;
	data.success = true;
	return data;
}

fileread::Data fileread::Func::loop_until_proper_files()
{
	fileread::Data input_data;
	input_data.success = false;
	while(!input_data.success)
	{
		std::cout << "FDK-reading input files" << std::endl;
		input_data = read_all_input_files();
		if(input_data.success)
		{
			std::cout << "FDK-Files are read correctly" << std::endl;
		}
		else
		{
			std::cout << "ERR-problem in input files, please correct the files and press [ENTER]" << std::endl;
			std::cin.get();
		}
	}
	return input_data;
}

