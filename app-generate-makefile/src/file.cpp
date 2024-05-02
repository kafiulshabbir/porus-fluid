#include "file.h"

std::string File::full_name() const
{
	return family_name + call_name;
}


std::string File::path_src() const
{
	return "app-simulation/src/";
}

std::string File::path_build() const
{
	return "app-simulation/build/";
}

std::string File::path_exe() const
{
	return "run/";
}


std::string File::cpp() const
{
	return path_src() + full_name() + ".cpp";
}

std::string File::object() const
{
	return path_build() + full_name() + ".o";
}

std::string File::head() const
{
	return path_src() + full_name() + ".h";
}

std::string File::exe() const
{
	return path_exe() + call_name;
}

std::vector<std::string> File::editable() const
{

	std::vector<std::string> v;
	if(has_head())
	{
		v.push_back(head());
	}
	if(has_cpp())
	{
		v.push_back(cpp());
	}

	return v;
}

std::vector<std::string> File::dependencies() const
{
	std::vector<std::string> v;
	if(has_head())
	{
		v.push_back(head());
	}
	if(has_cpp())
	{
		v.push_back(cpp());
	}

	v.push_back("app-simulation/src/declconst.h");

	return v;
}

bool File::has_object() const
{
	return !is_head();
}


bool File::is_head() const
{
	return this->type == Type::head;
}

bool File::is_lib() const
{
	return this->type == Type::lib;
}

bool File::is_exe() const
{
	return this->type == Type::exe;
}

bool File::has_head() const
{
	return is_head() || is_lib();
}

bool File::has_cpp() const
{
	return is_lib() || is_exe();
}

