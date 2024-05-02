#include "objects.h"

void Objects::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	for(const File& file: vfile)
	{
		if(!file.can_have_object())
		{
			continue;
		}

		print_object(fout, file);
	}
}

void Objects::print_object(std::ofstream& fout, const File& file)
{
	const std::string object = file.object();
	const std::string cpp = file.cpp();
	const std::vector<std::string> dependencies{cpp};

	Print::cmd(fout, object, dependencies);
	Print::echo(fout, "Executing command = compile/make object[.o] file for [" + cpp + "]");

	std::stringstream ss;
	ss << GENERAL_COMPILE << ' ' << cpp << " -o " << object;

	Print::script(fout, ss);
}
