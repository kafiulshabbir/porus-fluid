#include "cmdioread.h"

char cmdio::Read::command_char(const std::string& text_cmd)
{
	return command_general<char>(text_cmd);
}

bool cmdio::Read::command_char_exit_true(const std::string& text_promt, char& command_char)
{
	command_char = cmdio::Read::command_char(text_promt + ", e)xit");

	return command_char == 'e';
}
