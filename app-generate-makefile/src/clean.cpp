#include "clean.h"

void Clean::run(std::ofstream& fout, const DeclConst& decl_const)
{
	Print::cmd(fout, "clean");

	const std::string folder = decl_const.path.build;

	const std::string remove_dir = decl_const.terminal.rmrf;
	const std::string make_dir = decl_const.terminal.mkdir;

	const std::string remove = remove_dir + " " + folder;
	const std::string make = make_dir + " " + folder;


	Print::script(fout, remove);
	Print::script(fout, make);

	Print::echo(fout, "Command executed = clean");
}
