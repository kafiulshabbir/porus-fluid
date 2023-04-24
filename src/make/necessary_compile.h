#ifndef MF_NECESSARY_COMPILE_H
#define MF_NECESSARY_COMPILE_H

#include "make/file.h"
#include "make/utility.h"
#include "make/print.h"

class NecessaryCompile
{
	static void clear_plot_folder(std::ofstream& fout);
	
public:
	
	static void run(std::ofstream& fout, const std::vector<File>& vfile);
};


#endif
