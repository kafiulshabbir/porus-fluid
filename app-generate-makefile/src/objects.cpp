#include "objects.h"

void Objects::run(std::ofstream& fout, const DeclConst& decl_const)
{
	const std::vector<File>& file_vec = decl_const.file_vec;

	for(const File& file: file_vec)
	{
		if(!file.has_object())
		{
			continue;
		}

		print_object(fout, file, decl_const);
	}
}

void Objects::print_object(std::ofstream& fout, const File& file, const DeclConst& decl_const)
{
	const std::string cpp = file.cpp();
	const std::string object = file.object();
	const std::vector<std::string> dependencies = file.dependencies();

	Print::cmd(fout, object, dependencies);
	Print::echo(fout, "Executing command = compiling to make [" + object + "] from [" + cpp + "]");

	std::stringstream ss;
	ss << decl_const.compile_command.object << ' ' << cpp << " -o " << object;

	Print::script(fout, ss);
}
