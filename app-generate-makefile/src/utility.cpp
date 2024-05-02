#include "utility.h"

std::vector<File> Utility::VecFile::exe(const std::vector<File>& vfile)
{
	std::vector<File> v;
	for(const File& file: vfile)
	{
		if(file.is_exe())
		{
			v.push_back(file);
		}
	}

	return v;
}

std::vector<File> Utility::VecFile::object_all(const std::vector<File>& vfile)
{
	std::vector<File> v;

	for(const File& file: vfile)
	{
		if(file.has_object())
		{
			v.push_back(file);
		}
	}

	return v;
}


std::vector<File> Utility::VecFile::object_without_exe(const std::vector<File>& vfile)
{
	std::vector<File> v;

	for(const File& file: vfile)
	{
		if(file.is_head())
		{
			continue;

		}
		if(file.is_exe())
		{
			continue;
		}

		v.push_back(file);
	}

	return v;
}



std::vector<std::string> Utility::VecStr::edit(const std::vector<File>& vfile)
{
	std::vector<std::string> v;

	for(const File& file: vfile)
	{
		const std::vector<std::string> editable = file.editable();
		v.insert(v.end(), editable.cbegin(), editable.cend());
	}

	return v;
}

static std::vector<std::string> Utility::VecStr::exe(const std::vector<File> vfile)
{
	std::vector<std::string> v;
	for(const File& file: vfile)
	{
		v.push_back(file.exe());
	}

	return v;
}

static std::vector<std::string> Utility::VecStr::exe(const std::vector<File> vfile)
{
	std::vector<std::string> v;
	for(const File& file: vfile)
	{
		v.push_back(file.exe());
	}

	return v;
}

std::string Utility::Convert::str_from_vector(const std::vector<std::string>& vec_files)
{
	std::string s;
	for(const std::string& file: vec_files)
	{
		s += ' ';
		s += file;
	}

	return s;
}




