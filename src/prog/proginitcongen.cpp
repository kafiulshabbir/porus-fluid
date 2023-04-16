#include "fileio/fileioread.h"
#include "cmdio/cmdioread.h"
#include "initcon/initconmode.h"
#include "utility/utilityrandom.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

const std::string PROGRAM_PROPERTY_NAME = "proginitcongen initial_conditions_generator";

int main()
{
	utility::Random::seed();
	std::cout << PROGRAM_PROPERTY_NAME << std::endl;
	
	char command_main_menu;
	const std::string text_promt_main_menu = "g)enerate, m)odify";
	while(!initcon::Mode::Main::exit_code_main_menu(text_promt_main_menu, command_main_menu))
	{
		if(command_main_menu == 'g')
		{
			std::cout << "-generate mode" << std::endl;
			initcon::Mode::Main::generate();
		}
		else if(command_main_menu == 'm')
		{
			std::cout << "-modify mode" << std::endl;
			initcon::Mode::Main::modify();
		}
		else
		{
			std::cout << "-ERR-IPT-input command is not valid" << std::endl;
			continue;
		}
		// Display the status of the files
		const fileio::Data data = fileio::Read::all();
		if(data.success)
		{
			std::cout << "-FDK-All files are good" << std::endl;
		}
		else
		{
			std::cout << "-ERR-problems with input files" << std::endl;
		}
	}
	
	std::cout << "program exited: " << PROGRAM_PROPERTY_NAME << std::endl;
	
	return 0;
}
