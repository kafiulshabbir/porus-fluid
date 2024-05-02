#include "all.h"
#include "necessary_compile.h"
#include "force.h"
#include "clean.h"
#include "edit.h"
#include "executables.h"
#include "objects.h"

#include "declconst.h"


int main()
{
	const DeclConst decl_const;

	std::ofstream fout(decl_const.file_name_fout);

	// necessary compile
	All::run(fout);

	/*
	 * NecessaryCompile
	 * build all object files if needed
	 * link to form executables
	 * clean up the plot folder
	 */
	NecessaryCompile::run(fout, decl_const);

	/*
	 * Force
	 * clean the build folder
	 * run necessary compile
	 */
	Force::run(fout);

	/*
	 * Deletes the build folder
	 * remake the build folder
	 */
	Clean::run(fout, decl_const);

	/*
	 * Edit
	 * Command for geany to open all the files
	 */
	Edit::run(fout, decl_const);

	/*
	 * exe1: obj1.o obj2.o exe1.o
	 * 		linking -o run/exe1
	 */

	Executables::run(fout, decl_const);

	/*
	 * Objects
	 * Commands to build individual objects
	 * build/obj1.o: obj1.cpp obj1.h appsimulation.h
	 * 		g++ -c Wall -o obj1.o
	 */
	Objects::run(fout, decl_const);

	return 0;
}






