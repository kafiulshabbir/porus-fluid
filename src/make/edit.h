#ifndef MF_EDIT_H
#define MF_EDIT_H
	
#include "make/file.h"
#include "make/print.h"
#include "make/utility.h"

class Edit
{
	
public:
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};



#endif
