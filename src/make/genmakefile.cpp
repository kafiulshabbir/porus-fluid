#include "make/file.h"
#include "make/all.h"
#include "make/edit.h"
#include "make/gen.h"

#include <fstream>


const std::vector<File> VFILE
{
	{"cmdio",	"print",		Type::both},
	{"cmdio",	"read",			Type::both},
	
	{"decl",	"const",		Type::head},
	{"decl",	"filename", 	Type::head},
	{"decl",	"typedef",		Type::head},
	
	{"drw",		"",				Type::both},
	{"dst",		"diamension", 	Type::both},
	{"dst",		"mns",			Type::both},
	{"fileio",	"plot",			Type::both},
	{"fileio",	"read",			Type::both},
	{"fileio",	"write",		Type::both},
	{"func",	"connection", 	Type::both},
	{"func",	"determine",	Type::both},
	{"func",	"global",		Type::both},
	{"func",	"linear",		Type::both},
	
	{"initcon", "mode",			Type::both},
	{"prog",	"initcongen", 	Type::cpp},
	{"prog",	"simulate",		Type::cpp},
	
	{"utility", "random",		Type::both}
};


int main()
{
	std::ofstream fout("Makefile");
	All::run(fout, VFILE);
	Edit::run(fout, VFILE);
	Gen::run(fout);
	return 0;
}






