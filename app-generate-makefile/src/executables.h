#ifndef MF_EXECUTABLES_H
#define MF_EXECUTABLES_H

#include "print.h"
#include "file.h"

class Executables
{
	static void print_exe(
		std::ofstream& fout,
		const File& exe,
		const std::vector<File>& object_vec
	);

public:
	static void run(
		std::ofstream& fout,
		const DeclConst& decl_const
	);
};


#endif
