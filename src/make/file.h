#ifndef MF_FILE_H
#define MF_FILE_H

#include <string>
#include <vector>

struct File
{
	// enum class Type or File::Type
	// head: just app.h in src/
	// cpp: just app.cpp in src/
	// lib: app.cpp and app.h in src/
	// exe: only a c++ file with int main() {return 0;} program will be made in run/
	
	enum class Type{head, cpp, lib, exe};
	
	std::string folder;
	std::string partialname;
	Type type;
	
	std::string name() const;
	std::string path() const;
	std::string pathname() const;
	
	std::string cpp() const;
	std::string object() const;
	std::string head() const;
	std::string exe() const;
	
	std::vector<std::string> editable() const;	
	
	bool can_have_object() const;
};



#endif
