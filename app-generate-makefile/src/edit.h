#ifndef MF_EDIT_H
#define MF_EDIT_H

#include "file.h"
#include "print.h"
#include "utility.h"

class Edit
{

public:
	static void run(std::ofstream& fout, const DeclConst& decl_const);
};



#endif
