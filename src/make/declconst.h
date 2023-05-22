#ifndef MF_DECLCONST_H
#define MF_DECLCONST_H

#include "make/file.h"

const std::string GENERAL_COMPILE = "g++ -c -Isrc/ -Wall -std=c++17";
const std::string COMPILE_EXE = "g++ -Isrc/";

const std::vector<File> VFILE
{
	{"cmdio",	"print",		File::Type::lib},
	{"cmdio",	"read",			File::Type::lib},
	
	{"decl",	"const",		File::Type::head},
	{"decl",	"filename", 	File::Type::head},
	{"decl",	"typedef",		File::Type::head},
	
	{"drw",		"",				File::Type::lib},
	
	{"dst",		"diamension", 	File::Type::lib},
	{"dst",		"mns",			File::Type::lib},
	
	{"fileio",	"plot",			File::Type::lib},
	{"fileio",	"read",			File::Type::lib},
	{"fileio",	"write",		File::Type::lib},
	
	{"func",	"global",		File::Type::lib},
	{"func",	"pressure",		File::Type::lib},
	{"func",	"velocity",		File::Type::lib},
	{"func",	"timestep",		File::Type::lib},
	{"func",	"determine",	File::Type::lib},
	{"func",	"integration",	File::Type::lib},
	{"func",	"measure",		File::Type::lib},
	{"math",	"linear",		File::Type::lib},

	{"initcon", "mode",			File::Type::lib},
	
	{"prog",	"initcongen", 	File::Type::exe},
	{"prog",	"simulate",		File::Type::exe},
	{"prog",	"plot",			File::Type::exe},
	{"prog",	"color",		File::Type::exe},
	{"prog",	"test",			File::Type::exe},
	
	{"utility", "random",		File::Type::lib}
};

const std::vector<std::string> vmake
{
	"all",
	"clean",
	"edit",
	"executables",
	"file",
	"force",
	"gen",
	"genmakefile",
	"necessary_compile",
	"objects",
	"print",
	"utility"
};

#endif
