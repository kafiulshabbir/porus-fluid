#ifndef FILEREAD_H
#define FILEREAD_H

#include "dst/dstdiamension.h"
#include "decl/decltypedef.h"
#include "decl/declfilename.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <map>

namespace fileio
{
	struct Data
	{
		Tfloat radius;
		TMns mnsc;
		dst::Diamension diamension;
		bool success;
	};
	
	class Read
	{
		
		template<class T>
		static std::vector<std::vector<T>> table_from_linear(
			const std::vector<T>& lineardata, const int rows,
			const int cols);
		
		template<class T>
		static std::pair<std::vector<std::vector<T>>, bool> read_file(
			const std::string& file_name);
				
	public:
		static std::pair<Tfloat, bool> read_radius();
		static std::pair<TMns, bool> read_mnsc();
		static std::map<std::string, dst::Diamension> diamension();
		static Data all();
		static Data loop_until_proper_files();
	};
}	
	
template<class T>
std::vector<std::vector<T>> fileio::Read::table_from_linear(const std::vector<T>& lineardata, const int rows, const int cols)
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
std::pair<std::vector<std::vector<T>>, bool> fileio::Read::read_file(const std::string& file_name)
{
	std::ifstream fin(file_name);
	
	std::pair<std::vector<std::vector<T>>, bool> outdata;
	outdata.second = false;
	
	if(!fin)
	{
		std::cout << "-ERR-" << file_name << " does not exist, create using generate or restore manually." << std::endl;
		return outdata;
	}
	
	int rows, cols;
	fin >> rows >> cols;
	
	std::vector<T> lineardata;
	T ipt;
	while(fin >> ipt)
	{
		lineardata.push_back(ipt);
	}
	
	
	if(lineardata.size() != size_t(rows * cols))
	{
		std::cout << "-ERR-in file " << file_name << " rows, cols are declared differently than the actual data in them." << std::endl;
		return outdata;
	}
	
	outdata.first = table_from_linear(lineardata, rows, cols);
	outdata.second = true;
	
	return outdata;
}
	
#endif
