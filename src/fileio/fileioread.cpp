#include "fileio/fileioread.h"

std::pair<Tfloat, bool> fileio::Read::read_radius()
{
	return read_file<float>(declfilename::FILE_RADIUS);
}

std::pair<TMns, bool> fileio::Read::read_mnsc()
{
	return read_file<dst::Mns>(declfilename::FILE_MNSC);
}

fileio::Data fileio::Read::all()
{
	Data data;
	data.success = false;
	
	const auto radius = read_radius();
	if(!radius.second)
	{
		std::cout << "ERR-radius.txt is corrupted" << std::endl;
		return data;
	}
	data.radius = radius.first;
	
	const auto mnsc = read_mnsc();
	if(!mnsc.second)
	{
		std::cout << "ERR-mnsc.txt meniscus configuration file is corrupted" << std::endl;
		return data;
	}
	data.mnsc = mnsc.first;
	
	const dst::Diamension dradius(data.radius);
	const dst::Diamension dmnsc(data.mnsc);
	if(!(dradius == dmnsc))
	{
		std::cout << "ERR-Diamensions of radius.txt and mnsc.txt do not match" << std::endl;
		return data;
	}
	
	data.diamension = dradius;
	data.success = true;
	return data;
}

fileio::Data fileio::Read::loop_until_proper_files()
{
	fileio::Data input_data;
	input_data.success = false;
	while(!input_data.success)
	{
		std::cout << "FDK-reading input files" << std::endl;
		input_data = all();
		if(input_data.success)
		{
			std::cout << "FDK-Files are read correctly" << std::endl;
		}
		else
		{
			std::cout << "ERR-problem in input files, please correct the files and press [ENTER]" << std::endl;
			std::cin.get();
		}
	}
	return input_data;
}

std::map<std::string, dst::Diamension> fileio::Read::diamension()
{
	std::map<std::string, dst::Diamension> outmap;
	
	std::pair<Tfloat, bool> radius = read_radius();
	if(radius.second)
	{
		outmap.insert({declfilename::FILE_RADIUS, dst::Diamension(radius.first)});
	}
	else
	{
		std::cout << "-FDK-radius file is still bad" << std::endl;
	}
	
	std::pair<TMns, bool> mnsc = read_mnsc();
	if(mnsc.second)
	{
		outmap.insert({declfilename::FILE_MNSC, dst::Diamension(mnsc.first)});
	}
	else
	{
		std::cout << "-FDK-mnsc file is still bad" << std::endl;
		
	}
	
	return outmap;
}
