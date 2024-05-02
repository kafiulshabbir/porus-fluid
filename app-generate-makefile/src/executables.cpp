#include "executables.h"

void Executables::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	const std::vector<File>& vfile = decl_const.file_vec;

	const std::vector<File> exe_vec = Utility::VecFile::exe(vfile);
	const std::vector<File> object_without_exe_vec = VecFile::object_without_exe(vfile);

	for(const File& exe: exe_vec)
	{
		std::vector<File> object_vec_temp(object_without_exe_vec);
		object_vec_temp.push_back(exe);
		print_exe(fout, exe, object_vec_temp);
	}
}

void Executables::print_exe(std::ofstream& fout, const File& exe_file, const std::vector<File>& object_vec)
{
	const std::string exe_str = exe.exe();
	const std::vector<string> = Utility::VecStr::

	Print::cmd(fout, exe, vec_object);
	Print::echo(fout, "Executing command = link-compile of file [" + exe + "]");

	std::string str_object = Utility::str_from_vector(vec_object);

	std::stringstream ss;
	ss << "g++" << str_object << " -o " << exe;

	Print::script(fout, ss);
}
