#include "make/edit.h"

void Edit::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	Print::cmd(fout, "edit");
	
	const std::vector<std::string> vec_files_edit = Utility::vec_edit(vfile);
	const std::string str_files_edit = Utility::str_from_vector(vec_files_edit);
	
	std::stringstream ss;
	ss << "geany -i" << str_files_edit;
	
	Print::script(fout, ss);
	Print::echo(fout, "Command executed = open all files to be edited on Geany");
}
