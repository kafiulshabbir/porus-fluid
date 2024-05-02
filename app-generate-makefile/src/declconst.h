#ifndef MF_DECLCONST_H
#define MF_DECLCONST_H


namespace def_decl
{
	const std::vector<File> decl_const_file_vec
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

	const std::string decl_const_file_name_fout = "Makefile";

	const std::string path_app_folder = "app-simulation/";
	const std::string path_src_folder = "src/";
	const std::string path_build_folder = "build/";
	const std::string path_run_folder = "run/";
	const std::string path_plot = "output_files/plots";

	const std::string terminal_mkdir = "mkdir";
	const std::string terminal_rmrf = "rm -rf";
	const std::string terminal_geany_open = "geany -i";

	const std::string compile_command_object = "g++ -c -Wall -std=c++17";
	const std::string compile_command_exe = "g++";
}

class DeclConst
{
	class CompileCommand
	{
		std::string input_src;
	public:
		std::string object;
		std::string exe;

		CompileCommand(const std::string& path_src):
			input_src(" -I" + path_src),
			object(def_decl::compile_command_object + input_src),
			exe(def_decl::compile_command_exe + input_src)
		{}
	};

	class Path
	{
	public:
		std::string src;
		std::string build;
		std::string run;
		std::string plot;
		Path():
			src(def_decl::path_app_folder + def_decl::path_src_folder),
			build(def_decl::path_app_folder + def_decl::path_build_folder),
			run(def_decl::path_run_folder),
			plot(def_decl::path_run_folder + def_decl::path_plot)
		{}
	};

	class Terminal
	{
	public:
		std::string mkdir;
		std::string rmrf;
		std::string geany_open;

		Terminal():
			mkdir(def_decl::terminal_mkdir),
			rmrf(def_decl::terminal_rmrf),
			geany_open(def_decl::terminal_geany_open)
		{}
	};

public:
	std::string file_name_fout;
	std::vector<File> file_vec;
	Path path;
	Terminal terminal;
	CompileCommand compile_command;

	DeclConst():
		file_name_fout(def_decl::decl_const_file_name_fout),
		file_vec(def_decl::decl_const_file_vec),
		path(),
		terminal(),
		compile_command(path.src)
	{}
};



#endif
