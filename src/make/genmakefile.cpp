#include "make/all.h"
#include "make/necessary_compile.h"
#include "make/force.h"
#include "make/clean.h"
#include "make/edit.h"
#include "make/gen.h"
#include "make/executables.h"
#include "make/objects.h"

// all: necessary_compile
// force: clean necessary compile
// necessary_compile: vec of executables
// executable1: cmdvecobject
// executable2: cmdvecobject
// object1: cpp
// object2: cpp
// clean: [just delete object files]
	
int main()
{
	std::ofstream fout("Makefile");
	
	All::run(fout);
	NecessaryCompile::run(fout, VFILE);
	Force::run(fout);
	Clean::run(fout, VFILE);
	Edit::run(fout, VFILE);
	Gen::run(fout);
	Executables::run(fout, VFILE);
	Objects::run(fout, VFILE);

	return 0;
}






