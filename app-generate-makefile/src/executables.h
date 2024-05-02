#ifndef MF_EXECUTABLES_H
#define MF_EXECUTABLES_H

#include "print.h"
#include "declconst.h"

class Executables
{
	static void print_exe(
		std::ofstream& fout,
		const std::string& exe,
		const std::vector<std::string>& vec_object
	);

public:
	static void run(
		std::ofstream& fout,
		const DeclConst& decl_const
	);
};


#endif
