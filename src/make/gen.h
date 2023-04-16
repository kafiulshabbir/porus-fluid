#ifndef MF_GEN_H
#define MF_GEN_H

#include <fstream>

class Gen
{
	
public:
	static void run(std::ofstream& fout);
};


void Gen::run(std::ofstream& fout)
{
	fout << '\n' << "gen:" << '\n';
	fout << '\n' << "# Make the genmakefile which edites the makefile itself!" << '\n';
	fout << "\tg++ src/make/genmakefile.cpp -Isrc/ -o genmakefile" << '\n';
}


#endif
