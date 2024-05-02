#ifndef MF_UTILITY_H
#define MF_UTILITY_H

#include "file.h"
#include <string>
#include <vector>

class Utility
{


public:

	struct VecFile
	{
		static std::vector<File> exe(const std::vector<File>& vfile);
		static std::vector<File> object_all(const std::vector<File>& vfile);
		static std::vector<File> object_without_exe(const std::vector<File>& vfile);

	};

	struct VecStr
	{
		static std::vector<std::string> edit(const std::vector<File>& vfile)
		static std::vector<std::string> exe(const std::vector<File> vfile);
		static std::vector<std::string> object(const std::vector<File> vfile);
	};

	struct Convert
	{
		static std::string str_from_vector(const std::vector<std::string>& vec_files);
	};
};

#endif
