#include "make/utility.h"

std::vector<std::string> Utility::vec_exe(const std::vector<File>& vfile)
{
	std::vector<std::string> v;
	for(const File& file: vfile)
	{
		if(file.type == File::Type::exe)
		{
			v.push_back(file.exe());
		}
	}
	
	return v;
}

std::vector<File> Utility::vec_file_exe(const std::vector<File>& vfile)
{
	std::vector<File> v;
	for(const File& file: vfile)
	{
		if(file.type == File::Type::exe)
		{
			v.push_back(file);
		}
	}
	
	return v;
}

std::vector<std::string> Utility::vec_edit(const std::vector<File>& vfile)
{
	std::vector<std::string> v;
	
	for(const File& file: vfile)
	{
		const std::vector<std::string> editable = file.editable();
		v.insert(v.end(), editable.cbegin(), editable.cend());
	}
	
	return v;
}


std::vector<std::string> Utility::vec_object(const std::vector<File>& vfile)
{
	std::vector<std::string> v;
	
	for(const File& file: vfile)
	{
		if(file.can_have_object())
		{
			v.push_back(file.object());
		}
	}
	
	return v;
}


std::vector<std::string> Utility::vec_object_not_exe(const std::vector<File>& vfile)
{
	std::vector<std::string> v;
	
	for(const File& file: vfile)
	{
		if(!file.can_have_object())
		{
			continue;
			
		}
		if(file.type == File::Type::exe)
		{
			continue;
		}
		
		v.push_back(file.object());
	}
	
	return v;
}

std::string Utility::str_from_vector(const std::vector<std::string>& vec_files)
{
	std::string s;
	for(const std::string& file: vec_files)
	{
		s += ' ';
		s += file;
	}
	
	return s;
}
