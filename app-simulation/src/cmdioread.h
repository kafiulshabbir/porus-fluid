#ifndef CMDIOREAD_H
#define CMDIOREAD_H

#include <iostream>

namespace cmdio
{
	class Read
	{
		
	public:
		template<class T>
		static T command_general(const std::string& text_cmd);
		
		static char command_char(const std::string& text_cmd);
		static bool command_char_exit_true(const std::string& text_promt, char& command_char);
	};
}

template<class T>
T cmdio::Read::command_general(const std::string& text_cmd)
{
	std::cout << std::endl << "-CIN-" << text_cmd << ": ";
	
	T ipt;
	std::cin >> ipt;
	
	return ipt;
}

#endif
