#ifndef MF_CLEAN_H
#define MF_CLEAN_H

#include "print.h"
#include "declconst.h"

class Clean
{

public:
	static void run(std::ofstream& fout, const DeclConst& decl_const);
};


#endif