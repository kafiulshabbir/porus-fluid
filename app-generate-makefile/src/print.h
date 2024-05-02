#ifndef MF_PRINT_H
#define MF_PRINT_H

#include "utility.h"

#include <fstream>
#include <sstream>

class Print
{
public:
	static void echo(std::ofstream& fout, const std::string& s);
	static void cmd(std::ofstream& fout, const std::string& command_name, const std::vector<std::string>& vec_dependencies = {});
	static void script(std::ofstream& fout, const std::stringstream& str_script);
	static void script(std::ofstream& fout, const std::string& str_script);
};


#endif
