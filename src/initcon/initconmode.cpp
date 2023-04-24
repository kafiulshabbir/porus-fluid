#include "initcon/initconmode.h"

void initcon::Mode::Generate::radius(const dst::Diamension& diamension)
{
	const auto def_radius = cmdio::Read::general<float>(
		"default value of radius");
	const Tfloat r(diamension.rows, std::vector<float>(
		diamension.cols, def_radius));
	
	fileio::Write::run(r);
}

void initcon::Mode::Generate::mnsc(const dst::Diamension& diamension)
{
	const TMns m(diamension.rows,
		std::vector<dst::Mns>(diamension.cols));
		
	fileio::Write::run(m);
}

void initcon::Mode::Main::generate()
{
	cmdio::Print::diamension(); 
	// Ask for the diamensions
	std::cout << std::endl << "-CIN- (nrows, ncols): ";
	int nrows, ncols;
	std::cin >> nrows >> ncols;
	
	const dst::Diamension diamension(nrows, ncols);
	
	// Ask which file to rewrite
	const char cmd_file_regen = cmdio::Read::command(
		"r)adius, m)eniscus_configuration, b)oth, any_other_char)exit");
		
	if(cmd_file_regen == 'r')
	{
		Generate::radius(diamension);
	}
	else if(cmd_file_regen == 'm')
	{
		Generate::mnsc(diamension);
	}
	else if(cmd_file_regen == 'b')
	{
		Generate::radius(diamension);
		Generate::mnsc(diamension);
	}
	std::cout << "-EXT-exited generate" << std::endl;
}

void initcon::Mode::Main::modify()
{
	// Only works if the files are okay
	const char cmd_file_modify = cmdio::Read::command(
		"r)adius, m)eniscus, a)dd_random_to_radius, e)xit_or_other_char");
	
	if(cmd_file_modify == 'r')
	{
		Modify::radius();
	}
	else if(cmd_file_modify == 'm')
	{
		Modify::mnsc();
	}
	else if(cmd_file_modify == 'a')
	{
		Modify::add_random_to_radius();
	}
	std::cout << "-EXT-exited modify" << std::endl;
}
	
Tfloat initcon::Mode::Modify::add_random_temporary(Tfloat radius)
{
	const float random = declconst::FINE_RADIUS_RANDOMNESS;
	for(auto& row: radius)
	{
		for(auto& cell: row)
		{
			cell += utility::Random::fraction(random);
		}
	}
	
	return radius;
}

void initcon::Mode::Modify::add_random_to_radius()
{
	const std::string file_name = declfilename::FILE_RADIUS;
	const std::pair<Tfloat, bool> data = fileio::Read::read_radius();
	
	if(data.second == false)
	{
		std::cout << "-ERR-CORRUPTED file, fix the input files and try again, or generate first." << std::endl;
		return;
	}
	
	Tfloat radius = data.first;
	cmdio::Print::pmat(file_name + " original", radius);
	
	char command = 'r';
	do 
	{
		if(command == 'r')
		{
			radius = add_random_temporary(radius);
			cmdio::Print::pmat(file_name, radius);
		}
		else if(command == 's')
		{
			fileio::Write::run(radius);
			std::cout << "-FDK-File saved successful" << std::endl;
		}
		else
		{
			std::cout << "-ERR-CIN invalid command" << std::endl;
		}
	} while(!cmdio::Read::exit_loop_e_command("r)egenerate, s)ave", command));
}

void initcon::Mode::Modify::mnsc()
{
	const std::string file_name = declfilename::FILE_MNSC;
	const std::pair<TMns, bool> data = fileio::Read::read_mnsc();
	
	if(data.second == false)
	{
		std::cout << "-ERR-CORRUPTED file, fix the input files and try again, or generate first." << std::endl;
		return;
	}
	
	cmdio::Print::pmat(file_name, data.first);
	
	const auto modified_dst = modify_dst_from_cin(data.first);
	if(modified_dst.second)
	{
		std::cout << "-FDK-writing new data to file " << file_name << std::endl;
		fileio::Write::run(modified_dst.first);
	}
	else
	{
		std::cout << "-FDK-exited modify mode without saving changes" << std::endl;
	}
	
}

bool initcon::Mode::Modify::exit_code_modify_dst_save(char& save_yes_or_no)
{
	save_yes_or_no = cmdio::Read::command("WRR- Save before exit y/n");
	
	if(save_yes_or_no == 'y')
	{
		return true;
	}
	if(save_yes_or_no == 'n')
	{
		return true;
	}
	
	return false;
}

bool initcon::Mode::Modify::exit_code_modify_dst(const std::string& text_promt_modify_type_dst, char& command, bool& saved_dst)
{
	if(!cmdio::Read::exit_loop_e_command(text_promt_modify_type_dst, command))
	{
		return false;
	}
	
	if(saved_dst)
	{
		return true;
	}
	
	char save_yes_or_no;
	while(!exit_code_modify_dst_save(save_yes_or_no))
	{
		std::cout << "-ERR-CIN invalid command." << std::endl;
	}
	
	if(save_yes_or_no == 'y')
	{
		saved_dst = true; 
	}
	
	return true;
}

void initcon::Mode::Modify::radius()
{
	const std::string file_name = declfilename::FILE_RADIUS;
	const std::pair<Tfloat, bool> data = fileio::Read::read_radius();
	
	if(data.second == false)
	{
		std::cout << "-ERR-CORRUPTED file, fix the input files and try again, or generate first." << std::endl;
		return;
	}
	
	cmdio::Print::pmat(file_name, data.first);
	
	const auto modified_dst = modify_dst_from_cin(data.first);
	if(modified_dst.second)
	{
		std::cout << "-FDK-writing new data to file " << file_name << std::endl;
		fileio::Write::run(modified_dst.first);
	}
	else
	{
		std::cout << "-FDK-exited modify mode without saving changes" << std::endl;
	}
}

bool initcon::Mode::Main::exit_code_main_menu(const std::string& text_promt_main_menu, char& command)
{
	return cmdio::Read::exit_loop_e_command(text_promt_main_menu, command);
}
