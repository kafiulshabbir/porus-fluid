#ifndef DEF_DAT_FILL
#define DEF_DAT_FILL

#include "dec.h"
#include "stc_node.h"

namespace dat
{
	class cls_fill
	{
		std::string name_file;
		int n;
		
		matrix ver;
		matrix hor;
		
		int good_begin(int a, int b) const;
		double find(const node& node_i, const node& node_j) const;
		
	public:
		cls_fill() = delete;
		cls_fill(const std::string&, int);
		
		bool read_file();
		double operator() (const typ::node& first, const typ::node& second) const;
		double sign(const node& node_i, const node& node_j) const;
	};
}
#endif
