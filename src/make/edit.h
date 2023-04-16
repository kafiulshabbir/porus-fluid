#ifndef MF_EDIT_H
#define MF_EDIT_H

#include "make/file.h"

#include <fstream>
#include <vector>
#include <string>

class Edit
{
	static std::vector<std::string> list_files_edit(const std::vector<File>& vfile);
	
public:
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};

std::vector<std::string> Edit::list_files_edit(const std::vector<File>& vfile)
{
	std::vector<std::string> s;
	
	for(const File& file: vfile)
	{
		if(file.type == Type::both)
		{
			s.push_back(file.cpp());
			s.push_back(file.head());
		}
		else if(file.type == Type::head)
		{
			s.push_back(file.head());
		}
		else if(file.type == Type::cpp)
		{
			s.push_back(file.cpp());
		}
	}
	
	return s;	
}


void Edit::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	fout << '\n' << "edit:" << '\n';
	
	fout << '\n' << "# Requesting Geany to open the following files to be edited" << '\n';
	
	const std::string str_files_edit = File::str_from_vector(list_files_edit(vfile));
	fout << "\tgeany " << str_files_edit << '\n';
}


#endif
