#ifndef CMDIOREAD_H
#define CMDIOREAD_H

#include <iostream>

namespace cmdio
{
	class Read
	{
		
	public:
		template<class T>
		static T general(const std::string& text_cmd);
		
		static char command(const std::string& text_cmd);
		
		static bool exit_loop_e_command(std::string text_promt, char& command);
	};
}

template<class T>
static T cmdio::Read::general(const std::string& text_cmd)
{
	std::cout << std::endl << "-CIN-" << text_cmd << ": ";
	
	T ipt;
	std::cin >> ipt;
	
	return ipt;
}


#endif
