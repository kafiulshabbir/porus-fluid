#include "print.h"

void Print::echo(std::ofstream& fout, const std::string& s)
{
	fout << '\t' << "@echo " << '\"' << s << '\"' << '\n';
}

void Print::cmd(std::ofstream& fout, const std::string& s, const std::vector<std::string>& v)
{
	fout << "\n\n";
	fout << s << ':' << Utility::Convert::str_from_vector(v) << '\n';
	echo(fout, " ");
}

void Print::script(std::ofstream& fout, const std::stringstream& str_script)
{
	script(fout, str_script.str());
}

void Print::script(std::ofstream& fout, const std::string& str_script)
{
	fout << '\t' << str_script << '\n';
}
