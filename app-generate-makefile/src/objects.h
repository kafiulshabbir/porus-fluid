#ifndef MF_OBJECTS_H
#define MF_OBJECTS_H

#include "print.h"
#include "file.h"
#include "declconst.h"

class Objects
{
	static void print_object(std::ofstream& fout, const File& file);

public:
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};

#endif
