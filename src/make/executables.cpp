#include "make/executables.h"

void Executables::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	const std::vector<std::string> vec_object = Utility::vec_object(vfile);
	const std::vector<std::string> vec_exe = Utility::vec_exe(vfile);
	
	for(const std::string& exe: vec_exe)
	{
		print_exe(fout, exe, vec_object);
	}
}

void Executables::print_exe(std::ofstream& fout, const std::string& exe, const std::vector<std::string>& vec_object)
{
	Print::cmd(fout, exe, vec_object);
	Print::echo(fout, "Executing command = link-compile of file [" + exe + "]");
	
	std::string str_object = Utility::str_from_vector(vec_object);
	
	std::stringstream ss;
	ss << "g++" << str_object << " -o " << exe;
	
	Print::script(fout, ss);
}
