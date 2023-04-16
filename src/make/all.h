#ifndef MF_ALL_H
#define MF_ALL_H

#include "make/file.h"

#include <fstream>
#include <vector>

class All
{
	static void make_object_files(std::ofstream& fout, const std::vector<File>& vfile);
	static void clear_plot_folder(std::ofstream& fout);
	static void form_executables(std::ofstream& fout, const std::vector<File>& vfile);

public:	
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};


void All::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	fout << '\n' << "all:" << '\n';
	
	fout << '\n' << "\t# Making object files" << '\n';
	make_object_files(fout, vfile);
	
	fout << '\n' << "\t# Clear Plot Folder" << '\n';
	clear_plot_folder(fout);
	
	fout << '\n' << "\t# Create Executables" << '\n';
	form_executables(fout, vfile);
}

void All::make_object_files(std::ofstream& fout, const std::vector<File>& vfile)
{
	for(const auto& file: vfile)
	{
		if(file.type == Type::both || file.type == Type::cpp)
		{
			fout << '\t' << "g++ -c -std=c++17 " << file.cpp() << " -Isrc/ -o " << file.o() << '\n';
		}
	}
	
	fout << '\n';	
}

void All::clear_plot_folder(std::ofstream& fout)
{
	fout << "\trm -rf run/output_files/plots" << '\n';
	fout << "\tmkdir run/output_files/plots" << '\n';
}

void All::form_executables(std::ofstream& fout, const std::vector<File>& vfile)
{
	std::vector<std::string> link_object;
	std::vector<File> main_object;
	
	for(const auto& file: vfile)
	{
		if(file.type == Type::cpp)
		{
			main_object.push_back(file);
		}
		else if(file.type != Type::head)
		{
			link_object.push_back(file.o());
		}
	}
	
	const std::string str_link_object = File::str_from_vector(link_object);
	
	for(const auto& file_main: main_object)
	{
		fout << '\n' << "\t g++ " << file_main.o() << ' ' << str_link_object << "-o " << "run/" << file_main.full_name() << '\n';
	}
}


#endif
