#include "necessary_compile.h"

void NecessaryCompile::run(std::ofstream& fout, const DeclConst& decl_const)
{
	const std::vector<File>& file_vec = decl_const.file_vec;

	std::vector<File> exe_file_vec = Utility::FileVec::exe(file_vec);
	std::vector<std::string> exe_vec_str = Utility::StrVec::exe(exe_file_vec);

	Print::cmd(fout, "necessary_compile", exe_vec_str);
	Print::echo(fout, "Command executed = necessary_compile");

	clear_plot_folder(fout, decl_const);
}

void NecessaryCompile::clear_plot_folder(std::ofstream& fout, const DeclConst& decl_const)
{
	const std::string folder = decl_const.path.plot;

	const std::string remove_dir = decl_const.terminal.rmrf;
	const std::string make_dir = decl_const.terminal.mkdir;

	const std::string remove = remove_dir + " " + folder;
	const std::string make = make_dir + " " + folder;

	Print::script(fout, remove);
	Print::script(fout, make);
}
