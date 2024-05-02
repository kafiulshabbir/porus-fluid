#ifndef MF_DECLCONST_H
#define MF_DECLCONST_H

#include "file.h"

class DeclConst
{
	struct CompileCommand
	{
		std::string object_= "g++ -c -Isrc/ -Wall -std=c++17";
		std::string exe = "g++ -Isrc/";
	};

	class Path
	{
		std::string src_folder = "src/";
		std::string build_folder = "build/";
		std::string run_folder = "run/";

	public:
		std::string app_folder = "app-simulate/";
		std::string src = app_folder + src_folder;
		std::string build = app_folder + build_folder;
		std::string run = run_folder;
		std::string plot = run + "output_files/plots";
	};

	struct Terminal
	{
		std::string mkdir = "mkdir";
		std::string rmrf = "rm -rf";
		std::string geany_open = "geany -i";
	};

public:
	std::vector<File> file_vec
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

	CompileCommand compile_command;

	std::string file_name_fout = "Makefile";

	Path path;
	Terminal terminal;
};



#endif
