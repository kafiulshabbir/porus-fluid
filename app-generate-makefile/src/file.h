#ifndef MF_FILE_H
#define MF_FILE_H

#include <string>
#include <vector>

struct File
{
	// enum class Type or File::Type
	// head: just app.h in src/
	// lib: app.cpp and app.h in src/
	// exe: only a c++ file with int main() {return 0;} program will be made in run/

	enum class Type{head, lib, exe};

	std::string family_name;
	std::string call_name;
	Type type;

	std::string full_name() const;
	std::string path_src() const;
	std::string path_build() const;

	std::string cpp() const;
	std::string object() const;
	std::string head() const;
	std::string exe() const;

	std::vector<std::string> editable() const;

	bool is_head() const;
	bool is_lib() const;
	bool is_exe() const;

	bool has_head() const;
	bool has_cpp() const;
	bool has_object() const;
};



#endif
