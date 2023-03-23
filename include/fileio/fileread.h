#ifndef FILEREAD_H
#define FILEREAD_H

#include "dst/diamension.h"
#include "decs/simtypedefs.h"
#include "decs/filenames.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>

namespace fio
{
	struct Data
	{
		Tfloat radius;
		TMns mnsc;
		Diamension diamension;
		bool success;
	};
	
	class Read
	{
		
		template<class T>
		static std::vector<std::vector<T>> table_from_linear(const std::vector<T>& lineardata, const int rows, const int cols);
		
		template<class T>
		static std::pair<std::vector<std::vector<T>>, bool> read_file(const std::string& file_name);
			
		static std::pair<Tfloat, bool> read_radius();
		static std::pair<TMns, bool> read_mnsc();
		
	public:
		static Data read_all_input_files();
		static Data loop_until_proper_files();
	};
	
	
	
	
	
	template<class T>
	std::vector<std::vector<T>> Func::table_from_linear(const std::vector<T>& lineardata, const int rows, const int cols)
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
	std::pair<std::vector<std::vector<T>>, bool> Func::read_file(const std::string& file_name)
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
}

#endif
