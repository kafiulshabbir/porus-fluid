#ifndef MF_OBJECTS_H
#define MF_OBJECTS_H

#include "make/declconst.h"
#include "make/print.h"
#include "make/file.h"

class Objects
{
	static void print_object(std::ofstream& fout, const File& file);
	
public:
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};

#endif
