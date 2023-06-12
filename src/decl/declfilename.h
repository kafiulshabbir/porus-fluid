#ifndef DECLFILENAME_H
#define DECLFILENAME_H

namespace declfilename
{
	//Input Output of File names
	namespace sec
	{
		const std::string NAME_FOLDER_INPUT = "input_files/";
		const std::string NAME_FOLDER_OUTPUT = "output_files/";
		
		const std::string NAME_FILE_RADIUS = "radius.txt";
		const std::string NAME_FILE_MNSC = "mnsc.txt";
		const std::string NAME_FILE_FLUID_PPR = "fluid_ppr.txt";
		const std::string NAME_FILE_PLOTS = "plots/";
	}
	const std::string FILE_RADIUS = sec::NAME_FOLDER_INPUT + sec::NAME_FILE_RADIUS;
	const std::string FILE_MNSC = sec::NAME_FOLDER_INPUT + sec::NAME_FILE_MNSC ;
	const std::string FOLDER_PLOTS = sec::NAME_FOLDER_OUTPUT + sec::NAME_FILE_PLOTS;
	const std::string FILE_FLUID_PPR = sec::NAME_FOLDER_OUTPUT + sec::NAME_FILE_FLUID_PPR;
}

#endif
