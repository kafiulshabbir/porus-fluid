#include "make/executables.h"

void Executables::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	const std::vector<std::string> vec_object = Utility::vec_object_not_exe(vfile);
	const std::vector<File> vec_file_exe = Utility::vec_file_exe(vfile);
	
	for(const File& exe: vec_file_exe)
	{
		std::vector<std::string> vec_object_temp(vec_object);
		vec_object_temp.push_back(exe.object());
		print_exe(fout, exe.exe(), vec_object_temp);
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
