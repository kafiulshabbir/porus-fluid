#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "dst.h"

const std::string PROGRAM_PROPERTY_NAME = "initial_conditions_generator 04.03.2023";

template<class T>
T ReadCommand(const std::string& text_cmd)
{
	std::cout << "-CIN-" << text_cmd << ": ";
	T ipt;
	std::cin >> ipt;
	
	return ipt;
}


int main()
{
	std::cout << PROGRAM_PROPERTY_NAME << std::endl;
	
	char command_menu;
	while((command_menu = ReadCommand<char>("g)enerate, m)odify, e)xit")) != 'e')
	{
		if(command_menu == 'g')
		{
			std::cout << "-generate mode" << std::endl;	
		}
		else if(command_menu == 'm')
		{
			std::cout << "-modify mode" << std::endl;	
		}
		else
		{
			std::cout << "-ERR-IPT-input command is not valid" << std::endl;
		}
	}
	std::cout << "program exited" << std::endl;
	return 0;
}


