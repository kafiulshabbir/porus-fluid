#include "edit.h"

void Edit::run(std::ofstream& fout, const DeclConst& decl_const)
{
	Print::cmd(fout, "edit");

	const std::vector<File>& vfile = decl_const.file_vec;

	const std::vector<std::string> vec_files_edit = Utility::VecStr::edit(vfile);
	const std::string str_files_edit = Utility::Convert::str_from_vector(vec_files_edit);
	const std::string command_geany_open = decl_const.terminal.geany_open;

	std::stringstream ss;
	ss << command_geany_open << " " << str_files_edit;

	Print::script(fout, ss);
	Print::echo(fout, "Command executed = open all files to be edited on Geany");
}
