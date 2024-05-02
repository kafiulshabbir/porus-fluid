#include "edit.h"

void Edit::run(std::ofstream& fout, const DeclConst& decl_const)
{
	Print::cmd(fout, "edit");

	const std::vector<File>& file_vec = decl_const.file_vec;

	const std::vector<std::string> file_str_vec_edit = Utility::StrVec::edit(file_vec);
	const std::string str_files_edit = Utility::Str::from_str_vec(file_str_vec_edit);
	const std::string command_geany_open = decl_const.terminal.geany_open;

	std::stringstream ss;
	ss << command_geany_open << " " << str_files_edit;

	Print::script(fout, ss);
	Print::echo(fout, "Command executed = open all files to be edited on Geany");
}
