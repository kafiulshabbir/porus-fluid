#ifndef MF_EXECUTABLES_H
#define MF_EXECUTABLES_H

#include "make/file.h"
#include "make/print.h"
#include "make/utility.h"

class Executables
{
	static void print_exe(std::ofstream& fout,
		const std::string& exe,
		const std::vector<std::string>& vec_object);
	
public:
	static void run(std::ofstream& fout, 
		const std::vector<File>& vfile);
};


#endif
