#include "cmdio/cmdioread.h"


char cmdio::Read::command(const std::string& text_cmd)
{
	return general<char>(text_cmd);
}

bool cmdio::Read::exit_loop_e_command(std::string text_promt, char& command)
{
	text_promt += ", e)xit";
	command = cmdio::Read::command(text_promt);
	
	return command == 'e';
}
