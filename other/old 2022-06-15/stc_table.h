#ifndef DEF_STC_TABLE
#define DEF_STC_TABLE

#include "dec.h"
#include "stc_node.g"

namespace stc
{
	class cls_table
	{
		int n;
		matrix ver;
		matrix hor;
		
	public:
		cls_tube_frw() = default;
		double operator () (const stc::node& first, const stc::node& second) const;
	};


}
#endif
