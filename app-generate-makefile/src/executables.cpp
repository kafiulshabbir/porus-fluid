#include "executables.h"

void Executables::run(std::ofstream& fout, const DeclConst& decl_const)
{
	const std::vector<File>& file_vec = decl_const.file_vec;

	const std::vector<File> exe_vec = Utility::FileVec::exe(file_vec);
	const std::vector<File> object_without_exe_vec = Utility::FileVec::object_without_exe(file_vec);

	for(const File& exe: exe_vec)
	{
		std::vector<File> object_vec_temp(object_without_exe_vec);
		object_vec_temp.push_back(exe);
		print_exe(fout, exe, object_vec_temp);
	}
}

void Executables::print_exe(std::ofstream& fout, const File& exe_file, const std::vector<File>& object_vec)
{
	const std::string exe_str = exe_file.exe();
	const std::vector<std::string> object_str_vec = Utility::StrVec::object(object_vec);

	Print::cmd(fout, exe_str, object_str_vec);
	Print::echo(fout, "Executing command = link-compile of file [" + exe_str + "]");

	std::string object_str = Utility::Str::from_str_vec(object_str_vec);

	std::stringstream ss;
	ss << "g++" << object_str << " -o " << exe_str;

	Print::script(fout, ss);
}
