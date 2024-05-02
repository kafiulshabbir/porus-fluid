#include "utility.h"

std::vector<File> Utility::FileVec::exe(const std::vector<File>& file_vec)
{
	std::vector<File> v;
	for(const File& file: file_vec)
	{
		if(file.is_exe())
		{
			v.push_back(file);
		}
	}

	return v;
}

std::vector<File> Utility::FileVec::object_all(const std::vector<File>& file_vec)
{
	std::vector<File> v;

	for(const File& file: file_vec)
	{
		if(file.has_object())
		{
			v.push_back(file);
		}
	}

	return v;
}


std::vector<File> Utility::FileVec::object_without_exe(const std::vector<File>& file_vec)
{
	std::vector<File> v;

	for(const File& file: file_vec)
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



std::vector<std::string> Utility::StrVec::edit(const std::vector<File>& file_vec)
{
	std::vector<std::string> v;

	for(const File& file: file_vec)
	{
		const std::vector<std::string> editable = file.editable();
		v.insert(v.end(), editable.cbegin(), editable.cend());
	}

	return v;
}

std::vector<std::string> Utility::StrVec::exe(const std::vector<File>& file_vec)
{
	std::vector<std::string> v;
	for(const File& file: file_vec)
	{
		v.push_back(file.exe());
	}

	return v;
}

std::vector<std::string> Utility::StrVec::object(const std::vector<File>& file_vec)
{
	std::vector<std::string> v;
	for(const File& file: file_vec)
	{
		v.push_back(file.object());
	}

	return v;
}

std::string Utility::Str::from_str_vec(const std::vector<std::string>& file_str_vec)
{
	std::string s;
	for(const std::string& file: file_str_vec)
	{
		s += ' ';
		s += file;
	}

	return s;
}




