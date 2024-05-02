#ifndef MF_UTILITY_H
#define MF_UTILITY_H

#include "file.h"
#include <string>
#include <vector>

class Utility
{
public:
	struct FileVec
	{
		static std::vector<File> exe(const std::vector<File>& file_vec);
		static std::vector<File> object_all(const std::vector<File>& file_vec);
		static std::vector<File> object_without_exe(const std::vector<File>& file_vec);
	};

	struct StrVec
	{
		static std::vector<std::string> edit(const std::vector<File>& file_vec);
		static std::vector<std::string> exe(const std::vector<File>& file_vec);
		static std::vector<std::string> object(const std::vector<File>& file_vec);
	};

	struct Str
	{
		static std::string from_str_vec(const std::vector<std::string>& file_str_vec);
	};
};

#endif
