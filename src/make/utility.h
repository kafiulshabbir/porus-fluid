#ifndef MF_UTILITY_H
#define MF_UTILITY_H

#include "make/file.h"
#include <string>
#include <vector>

class Utility
{
public:

	static std::vector<std::string> vec_exe(const std::vector<File>& vfile);
	static std::vector<std::string> vec_edit(const std::vector<File>& vfile);
	static std::vector<std::string> vec_object(const std::vector<File>& vfile);
	static std::string str_from_vector(const std::vector<std::string>& vec_files);
};

#endif
