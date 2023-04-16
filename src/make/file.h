#ifndef MF_FILE_H
#define MF_FILE_H

#include <string>
#include <vector>

enum class Type{head, cpp, both};

struct File
{
	std::string folder;
	std::string name;
	Type type;
	
	std::string full_name() const;
	std::string path() const;
	std::string cpp() const;
	std::string o() const;
	std::string head() const;
	
	static std::string str_from_vector(const std::vector<std::string>& list_files);
};


std::string File::full_name() const
{
	return folder + name;
}

std::string File::path() const
{
	return "src/" + folder + "/" + full_name();
}

std::string File::cpp() const
{
	return path() + ".cpp";
}

std::string File::o() const
{
	return "build/" + full_name() + ".o";
}

std::string File::head() const
{
	return path() + ".h";
}


std::string File::str_from_vector(const std::vector<std::string>& list_files)
{
	std::string s;
	for(const auto& file: list_files)
	{
		s += file + ' ';
	}
	
	return s;
}

#endif
