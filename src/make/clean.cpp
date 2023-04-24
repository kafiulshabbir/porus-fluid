#include "make/clean.h"

void Clean::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	Print::cmd(fout, "clean");
	
	const std::vector<std::string> vec_files = Utility::vec_object(vfile);
	const std::string str_files = Utility::str_from_vector(vec_files);
	
	std::stringstream ss;
	ss << "rm -f" << str_files;
	
	Print::script(fout, ss);
	
	Print::echo(fout, "Command executed = clean");
}
