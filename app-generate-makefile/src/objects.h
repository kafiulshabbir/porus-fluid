#ifndef MF_OBJECTS_H
#define MF_OBJECTS_H

#include "print.h"
#include "file.h"

class Objects
{
	static void print_object(std::ofstream& fout, const File& file, const DeclConst& decl_const);

public:
	static void run(std::ofstream& fout, const DeclConst& decl_const);
};

#endif
