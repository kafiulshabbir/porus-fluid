#ifndef MF_CLEAN_H
#define MF_CLEAN_H

#include "make/file.h"
#include "make/print.h"
#include "make/utility.h"

class Clean
{
	
public:
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};


#endif
