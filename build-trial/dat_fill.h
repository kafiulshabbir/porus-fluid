#ifndef DEF_DAT_FILL
#define DEF_DAT_FILL

#include "dec.h"
#include "dat_tube.h"

namespace dat
{
	class cls_fill: public cls_tube_frw
	{

		
	public:
		cls_fill() = delete;
		cls_fill(const std::string& name_file, int n, double multiplier, double left_default, double right_default);
	};
}
#endif
