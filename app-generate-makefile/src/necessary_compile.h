#ifndef MF_NECESSARY_COMPILE_H
#define MF_NECESSARY_COMPILE_H

#include "print.h"
#include "declconst.h"

class NecessaryCompile
{
	static void clear_plot_folder(std::ofstream& fout, const DeclConst& decl_const);

public:

	static void run(std::ofstream& fout, const DeclConst& decl_const);
};


#endif
