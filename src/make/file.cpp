#include "make/file.h"

std::string File::name() const
{
	return folder + partialname;
}

std::string File::path() const
{
	return "src/" + folder + "/";
}

std::string File::pathname() const
{
	return path() + name();
}

std::string File::cpp() const
{
	return pathname() + ".cpp";
}

std::string File::object() const
{
	return "build/" + name() + ".o";
}

std::string File::head() const
{
	return pathname() + ".h";
}

std::string File::exe() const
{
	return "run/" + partialname;
}

std::vector<std::string> File::editable() const
{
	bool exists_head = false;
	bool exists_cpp = false;
	
	if(type == Type::lib)
	{
		exists_head = true;
		exists_cpp = true;
	}
	else if(type == Type::head)
	{
		exists_head = true;
	}
	else
	{
		exists_cpp = true;
	}	

	std::vector<std::string> v;
	if(exists_head)
	{
		v.push_back(head());
	}
	if(exists_cpp)
	{
		v.push_back(cpp());
	}
	
	return v;
}

bool File::can_have_object() const
{
	return type != Type::head;
}
