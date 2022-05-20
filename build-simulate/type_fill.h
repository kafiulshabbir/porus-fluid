#ifndef DEF_TYPE_FILL
#define DEF_TYPE_FILL

#include "project_includes.h"
#include "type_node.h"

namespace typ
{
	class type_fill
	{
		std::string name_file;
		int n;
		
		matrix ver;
		matrix hor;

	public:
		type_fill() = delete;
		type_fill(const std::string&, int);
		
		bool read_file();
		double operator() (const typ::node& first, const typ::node& second);
	};
}
#endif
